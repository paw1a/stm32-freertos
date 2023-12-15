#include "startup.h"
#include "common.h"
#include "system_stm32f4xx.h"

#define STACK_SIZE 1024

extern void main(void);

static inline void InitDataAndBSS(void);

void HardFault_Handler(void);
void NMI_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

/*
 * Reserve space for the system stack. MPU can protect
 * an aligned address at 32 bytes.
 */
#if defined(__ICCARM__)
#pragma data_alignment = 32
__no_init static uint32_t Stack[STACK_SIZE] @ "CSTACK";
#elif defined(__GNUC__)
static __attribute__((section(".stackarea"))) uint32_t Stack[STACK_SIZE] __attribute__((aligned(32)));
#else
#error "Compiler is undefined!"
#endif

static inline void InitDataAndBSS(void) {
#ifdef __GNUC__
  extern uint32_t __data_start;
  extern uint32_t __data_end;
  extern uint32_t __data_init;
  extern uint32_t __bss_start;
  extern uint32_t __bss_end;
  extern uint32_t __textrw_start;
  extern uint32_t __textrw_end;
  extern uint32_t __textrw_init;

  uint32_t *_data = &__data_start;
  uint32_t *_edata = &__data_end;
  uint32_t *_data_init = &__data_init;
  uint32_t *_bss = &__bss_start;
  uint32_t *_ebss = &__bss_end;
  uint32_t *_textrw = &__textrw_start;
  uint32_t *_etextrw = &__textrw_end;
  uint32_t *_textrw_init = &__textrw_init;
#elif __ICCARM__
#pragma section = ".data"
#pragma section = ".data_init"
#pragma section = ".bss"
#pragma section = ".textrw"
#pragma section = ".textrw_init"
  uint32_t *_data = __section_begin(".data");
  uint32_t *_edata = __section_end(".data");
  uint32_t *_data_init = __section_begin(".data_init");
  uint32_t *_bss = __section_begin(".bss");
  uint32_t *_ebss = __section_end(".bss");
  uint32_t *_textrw = __section_begin(".textrw");
  uint32_t *_etextrw = __section_end(".textrw");
  uint32_t *_textrw_init = __section_begin(".textrw_init");
#endif // __ICCARM__

  uint32_t n;

  /*
   * Copy the data segment initializers from flash to SRAM.
   */
  n = _edata - _data;

  while (n--) {
    *_data++ = *_data_init++;
  }

  /*
   * Copy the textrw segment initializers from flash to SRAM.
   */
  n = _etextrw - _textrw;

  while (n--) {
    *_textrw++ = *_textrw_init++;
  }

  /*
   * Zero fill the bss segment.
   */
  n = _ebss - _bss;

  while (n--) {
    *_bss++ = 0x00UL;
  }
}

/*
 * This function must be non-optimized, because the medium optimization
 * makes code that have bad behavior with SP and LR registers,
 * when there is stack in the end of memory.
 */
#ifdef __ICCARM__
#pragma optimize = none
#endif // __ICCARM__
void Reset_Handler(void) {
  /*
   * Run data and bss initialization (global variables initial values).
   */
  InitDataAndBSS();

  /*
   * Setup the microcontroller system. RST clock configuration to
   * the default reset state. Setup SystemCoreClock variable.
   */
  SystemInit();

  /*
   * Call the application's entry point.
   */
  main();
}

/*
 * The vector table.  Note that the proper constructs must be placed on this to
 * ensure that it ends up at physical address 0x0000.0000.
 */
#if defined(__ICCARM__)
__root void (*const __vector_table[])(void) @ ".intvec" =
#elif defined(__GNUC__)
void (*const __vector_table[])(void) __attribute__((section(".isr_vector"))) =
#endif // __ICCARM__
  {
    (void (*)(void))((uint32_t)Stack + sizeof(Stack)),
    // The initial stack pointer
    Reset_Handler,     // Reset Handler
    NMI_Handler,       // NMI Handler
    HardFault_Handler, // Hard Fault Handler
    IntDefaultHandler, // MPU Fault Handler
    IntDefaultHandler, // Bus Fault Handler
    IntDefaultHandler, // Usage Fault Handler
    0,                 // Reserved
    IntDefaultHandler, // Reserved
    IntDefaultHandler, // Reserved
    0,                 // Reserved
    SVC_Handler,       // SVCall Handler
    IntDefaultHandler, // Debug Monitor Handler
    0,                 // Reserved
    PendSV_Handler,    // PendSV Handler
    SysTick_Handler,   // SysTick Handler
    /*
     * External interrupts.
     */
    IntDefaultHandler, //< WWDG_IRQHandler,
    IntDefaultHandler, //< PVD_PVM_IRQHandler,
    IntDefaultHandler, //< RTC_TAMP_LSECSS_IRQHandler,
    IntDefaultHandler, //< RTC_WKUP_IRQHandler,
    IntDefaultHandler, //< FLASH_IRQHandler,
    IntDefaultHandler, //< RCC_IRQHandler,
    IntDefaultHandler, //< EXTI0_IRQHandler,
    IntDefaultHandler, //< EXTI1_IRQHandler,
    IntDefaultHandler, //< EXTI2_IRQHandler,
    IntDefaultHandler, //< EXTI3_IRQHandler,
    IntDefaultHandler, //< EXTI4_IRQHandler,
    IntDefaultHandler, //< DMA1_Channel1_IRQHandler,
    IntDefaultHandler, //< DMA1_Channel2_IRQHandler,
    IntDefaultHandler, //< DMA1_Channel3_IRQHandler,
    IntDefaultHandler, //< DMA1_Channel4_IRQHandler,
    IntDefaultHandler, //< DMA1_Channel5_IRQHandler,
    IntDefaultHandler, //< DMA1_Channel6_IRQHandler,
    IntDefaultHandler, //< DMA1_Channel7_IRQHandler,
    IntDefaultHandler, //< ADC1_2_IRQHandler,
    IntDefaultHandler, //< USB_HP_IRQHandler,
    IntDefaultHandler, //< USB_LP_IRQHandler,
    IntDefaultHandler, //< FDCAN1_IT0_IRQHandler,
    IntDefaultHandler, //< FDCAN1_IT1_IRQHandler,
    IntDefaultHandler, //< EXTI9_5_IRQHandler,
    IntDefaultHandler, //< TIM1_BRK_TIM15_IRQHandler,
    IntDefaultHandler, //< TIM1_UP_TIM16_IRQHandler,
    IntDefaultHandler, //< TIM1_TRG_COM_TIM17_IRQHandler,
    IntDefaultHandler, //< TIM1_CC_IRQHandler,
    IntDefaultHandler, //< TIM2_IRQHandler,
    IntDefaultHandler, //< TIM3_IRQHandler,
    IntDefaultHandler, //< TIM4_IRQHandler,
    IntDefaultHandler, //< I2C1_EV_IRQHandler,
    IntDefaultHandler, //< I2C1_ER_IRQHandler,
    IntDefaultHandler, //< I2C2_EV_IRQHandler,
    IntDefaultHandler, //< I2C2_ER_IRQHandler,
    IntDefaultHandler, //< SPI1_IRQHandler,
    IntDefaultHandler, //< SPI2_IRQHandler,
    IntDefaultHandler, //< USART1_IRQHandler,
    IntDefaultHandler, //< USART2_IRQHandler,
    IntDefaultHandler, //< USART3_IRQHandler,
    IntDefaultHandler, //< EXTI15_10_IRQHandler,
    IntDefaultHandler, //< RTC_Alarm_IRQHandler,
    IntDefaultHandler, //< USBWakeUp_IRQHandler,
    IntDefaultHandler, //< TIM8_BRK_IRQHandler,
    IntDefaultHandler, //< TIM8_UP_IRQHandler,
    IntDefaultHandler, //< TIM8_TRG_COM_IRQHandler,
    IntDefaultHandler, //< TIM8_CC_IRQHandler,
    IntDefaultHandler, //< ADC3_IRQHandler,
    IntDefaultHandler, //< FMC_IRQHandler,
    IntDefaultHandler, //< LPTIM1_IRQHandler,
    IntDefaultHandler, //< TIM5_IRQHandler,
    IntDefaultHandler, //< SPI3_IRQHandler,
    IntDefaultHandler, //< UART4_IRQHandler,
    IntDefaultHandler, //< UART5_IRQHandler,
    IntDefaultHandler, //< TIM6_DAC_IRQHandler,
    IntDefaultHandler, //< TIM7_DAC_IRQHandler,
    IntDefaultHandler, //< DMA2_Channel1_IRQHandler,
    IntDefaultHandler, //< DMA2_Channel2_IRQHandler,
    IntDefaultHandler, //< DMA2_Channel3_IRQHandler,
    IntDefaultHandler, //< DMA2_Channel4_IRQHandler,
    IntDefaultHandler, //< DMA2_Channel5_IRQHandler,
    IntDefaultHandler, //< ADC4_IRQHandler,
    IntDefaultHandler, //< ADC5_IRQHandler,
    IntDefaultHandler, //< UCPD1_IRQHandler,
    IntDefaultHandler, //< COMP1_2_3_IRQHandler,
    IntDefaultHandler, //< COMP4_5_6_IRQHandler,
    IntDefaultHandler, //< COMP7_IRQHandler,
    IntDefaultHandler, //< HRTIM1_Master_IRQHandler,
    IntDefaultHandler, //< HRTIM1_TIMA_IRQHandler,
    IntDefaultHandler, //< HRTIM1_TIMB_IRQHandler,
    IntDefaultHandler, //< HRTIM1_TIMC_IRQHandler,
    IntDefaultHandler, //< HRTIM1_TIMD_IRQHandler,
    IntDefaultHandler, //< HRTIM1_TIME_IRQHandler,
    IntDefaultHandler, //< HRTIM1_FLT_IRQHandler,
    IntDefaultHandler, //< HRTIM1_TIMF_IRQHandler,
    IntDefaultHandler, //< CRS_IRQHandler,
    IntDefaultHandler, //< SAI1_IRQHandler,
    IntDefaultHandler, //< TIM20_BRK_IRQHandler,
    IntDefaultHandler, //< TIM20_UP_IRQHandler,
    IntDefaultHandler, //< TIM20_TRG_COM_IRQHandler,
    IntDefaultHandler, //< TIM20_CC_IRQHandler,
    IntDefaultHandler, //< FPU_IRQHandler,
    IntDefaultHandler, //< I2C4_EV_IRQHandler,
    IntDefaultHandler, //< I2C4_ER_IRQHandler,
    IntDefaultHandler, //< SPI4_IRQHandler,
    IntDefaultHandler, //< 0,
    IntDefaultHandler, //< FDCAN2_IT0_IRQHandler,
    IntDefaultHandler, //< FDCAN2_IT1_IRQHandler,
    IntDefaultHandler, //< FDCAN3_IT0_IRQHandler,
    IntDefaultHandler, //< FDCAN3_IT1_IRQHandler,
    IntDefaultHandler, //< RNG_IRQHandler,
    IntDefaultHandler, //< LPUART1_IRQHandler,
    IntDefaultHandler, //< I2C3_EV_IRQHandler,
    IntDefaultHandler, //< I2C3_ER_IRQHandler,
    IntDefaultHandler, //< DMAMUX_OVR_IRQHandler,
    IntDefaultHandler, //< QUADSPI_IRQHandler,
    IntDefaultHandler, //< DMA1_Channel8_IRQHandler,
    IntDefaultHandler, //< DMA2_Channel6_IRQHandler,
    IntDefaultHandler, //< DMA2_Channel7_IRQHandler,
    IntDefaultHandler, //< DMA2_Channel8_IRQHandler,
    IntDefaultHandler, //< CORDIC_IRQHandler,
    IntDefaultHandler, //< FMAC_IRQHandler,
    IntDefaultHandler, //< 0,
    IntDefaultHandler, //< 0,
    IntDefaultHandler, //< 0,
    IntDefaultHandler, //< 0,
    IntDefaultHandler, //< 0,
    IntDefaultHandler, //< 0,
    IntDefaultHandler, //< 0,
    IntDefaultHandler, //< 0,
    IntDefaultHandler, //< 0,
    IntDefaultHandler, //< 0,
  };

/*
 * This is the code that gets called when the processor receives a NMI. This
 * simply enters an infinite loop, preserving the system state for examination
 * by a debugger.
 */
__weak void NMI_Handler(void) {
  while (1) {
    // Capture
  }
}

/*
 * This is the code that gets called when the processor receives a fault
 * interrupt. This simply enters an infinite loop, preserving the system state
 * for examination by a debugger.
 */
__weak void HardFault_Handler(void) {
  while (1) {
    // Capture
  }
}

__weak void SVC_Handler(void) {
  while (1) {
  }
}

__weak void PendSV_Handler(void) {
  while (1) {
  }
}

__weak void SysTick_Handler(void) {
  while (1) {
  }
}

/*
 * This is the code that gets called when the processor receives an unexpected
 * interrupt. This simply enters an infinite loop, preserving the system state
 * for examination by a debugger.
 */
__weak void IntDefaultHandler(void) {
  while (1) {
  }
}
