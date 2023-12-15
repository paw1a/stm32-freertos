#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include <assert.h>

#define BIT_0  (1 << 0)
#define BIT_1  (1 << 1)
#define BIT_2  (1 << 2)
#define BIT_3  (1 << 3)
#define BIT_4  (1 << 4)
#define BIT_5  (1 << 5)
#define BIT_6  (1 << 6)
#define BIT_7  (1 << 7)
#define BIT_8  (1 << 8)
#define BIT_9  (1 << 9)
#define BIT_10 (1 << 10)
#define BIT_11 (1 << 11)
#define BIT_12 (1 << 12)
#define BIT_13 (1 << 13)
#define BIT_14 (1 << 14)
#define BIT_15 (1 << 15)
#define BIT_16 (1 << 16)
#define BIT_17 (1 << 17)
#define BIT_18 (1 << 18)
#define BIT_19 (1 << 19)
#define BIT_20 (1 << 20)
#define BIT_21 (1 << 21)
#define BIT_22 (1 << 22)
#define BIT_23 (1 << 23)
#define BIT_24 (1 << 24)
#define BIT_25 (1 << 25)
#define BIT_26 (1 << 26)
#define BIT_27 (1 << 27)
#define BIT_28 (1 << 28)
#define BIT_29 (1 << 29)
#define BIT_30 (1 << 30)
#define BIT_31 (1 << 31)

#define _2_BITS  0x3
#define _3_BITS  0x7
#define _4_BITS  0xf
#define _5_BITS  0x1f
#define _6_BITS  0x3f
#define _7_BITS  0x7f
#define _8_BITS  0xff
#define _9_BITS  0x1ff
#define _10_BITS 0x3ff
#define _11_BITS 0x7ff
#define _12_BITS 0xfff
#define _13_BITS 0x1fff
#define _14_BITS 0x3fff
#define _15_BITS 0x7fff
#define _16_BITS 0xffff
#define _17_BITS 0x1ffff
#define _18_BITS 0x3ffff
#define _19_BITS 0x7ffff
#define _20_BITS 0xfffff
#define _21_BITS 0x1fffff
#define _22_BITS 0x3fffff
#define _23_BITS 0x7fffff
#define _24_BITS 0xffffff
#define _25_BITS 0x1ffffff
#define _26_BITS 0x3ffffff
#define _27_BITS 0x7ffffff
#define _28_BITS 0xfffffff
#define _29_BITS 0x1fffffff
#define _30_BITS 0x3fffffff
#define _31_BITS 0x7fffffff
#define _32_BITS 0xffffffff

#define NO_TIMEOUT 0xffffffff

#define swap(x, y) \
  {                \
    (x) ^= (y);    \
    (y) ^= (x);    \
    (x) ^= (y);    \
  }

#define clamp(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#define increase(x, low, high) (((x) == high) ? (low) : ((x) + 1))
#define decrease(x, low, high) (((x) == low) ? (high) : ((x)-1))
#define limit_up(x, lim)       ((x) = (((x) > (lim)) ? (lim) : (x)))
#define limit_down(x, lim)     ((x) = (((x) > (lim)) ? (x) : (lim)))

#define set_bits(var, bits)         ((var) |= (bits))
#define reset_bits(var, bits)       ((var) &= ~(bits))
#define is_bit_set(var, bit)        ((var) & (bit))
#define are_all_bits_set(var, bits) (((var) & (bits)) == (bits))
#define is_any_bit_set(var, bits)   is_bit_set((var), (bits))

#if defined(__GNUC__)
#define __no_init  __attribute__((section(".noinit")))
#define __noreturn __attribute__((noreturn))
#define __ramfunc  __attribute__((long_call, section(".ramfunc")))
#define __weak     __attribute__((weak))
#endif // __GNUC__

#if ((defined __GNUC__) && (CPU_CORE == ARM_CORTEX_M))
#define __optimize_size __attribute__((optimize("O0")))
#else
#define __optimize_size
#endif

void GlblErrorCatch(void);
uint32_t GCD(uint32_t A, uint32_t B);
void EmptyFunction(void);
