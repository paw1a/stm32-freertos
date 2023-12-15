TOOLCHAIN = arm-none-eabi-
CC      := $(TOOLCHAIN)gcc
LD      := $(TOOLCHAIN)ld
AS      := $(TOOLCHAIN)as
OBJCOPY := $(TOOLCHAIN)objcopy

TARGET := build/firmware.elf

LINKER_SCRIPT := linker.ld

DEFFLAGS := -DDEBUG -DSTM32F407xx
ASFLAGS = -msyntax=att --warn --fatal-warnings

CFLAGS  = -I. \
					-Wextra -Wno-sign-compare -Wpacked \
					-O0 -g -std=c11 -fno-strict-aliasing -fno-strict-overflow \
					-Wno-main -ffunction-sections -fdata-sections -fwrapv \
					--specs=nosys.specs  -mcpu=cortex-m4 -mfloat-abi=hard \
					-mfpu=fpv4-sp-d16 \

LDFLAGS = -nostartfiles -Wl,-static -Wl,--gc-sections \
					-Wl,-cref -Wl,--print-memory-usage -Wl,-T,$(LINKER_SCRIPT) \
					-Wl,-Map,build/memory.map \

INCLUDE_DIRS := include \
								include/stm32f4xx/include \
								cmsis/include \
								freertos/include \
								freertos/cmsis-rtos2/include \
								freertos/portable/gcc/armcm4f \
								stm32f4xx-hal/include \
								stm32f4xx-hal/include/legacy \

INCLUDES = $(INCLUDE_DIRS:%=-I%)

SRC_DIRS := src \
						stm32f4xx-hal/src \
						include/stm32f4xx/src/gcc \
						freertos \
						freertos/cmsis-rtos2/src \
						freertos/portable/common \
						freertos/portable/gcc/armcm4f \

SRCS_EXTRA := freertos/portable/mem/heap_5.c

OBJ_DIR := build/obj

SRCS := $(foreach DIR,$(SRC_DIRS),$(wildcard $(DIR)/*.c))
SRCS += $(foreach DIR,$(SRC_DIRS),$(wildcard $(DIR)/*.s))
SRCS += $(SRCS_EXTRA)
OBJS := $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS) $(LINKER_SCRIPT)
	@echo "BUILD $<"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o: %.c
	@echo "CC $<"
	@mkdir -p $(@D)
	@$(CC) -c $(CFLAGS) $(DEFFLAGS) $(INCLUDES) -o $@ $<

$(OBJ_DIR)/%.o: %.s
	@echo "AS $<"
	@$(CC) -c $(CFLAGS) -o $@ $<

%.bin: %.elf
	@echo "OBJCOPY $@"
	@$(OBJCOPY) -O binary $^ $@

flash: build/firmware.bin
	st-flash --reset write $< 0x8000000

format:
	find . \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} \;

clean:
	rm -rf build

debug:
	@echo $(OBJS)

.PHONY: all clean format debug flash
