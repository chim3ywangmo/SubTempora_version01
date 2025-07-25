/* platform.h: Cross-platform macros and compatibility shims. */

#pragma once

#include <stdint.h>

#if defined(_MSC_VER)
#define ALWAYS_INLINE __forceinline
#define CONST_FUNCTION
#define PURE_FUNCTION
#define __restrict__ __restrict
#define SIMDCOMP_ALIGNED(x) __declspec(align(x))
#else
#if defined(__GNUC__)
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#define CONST_FUNCTION __attribute__((const))
#define PURE_FUNCTION __attribute__((pure))
#define SIMDCOMP_ALIGNED(x) __attribute__((aligned(x)))
#endif
#endif

#ifdef _MSC_VER
#include <intrin.h>

int __inline __builtin_clz(uint32_t value) {
  unsigned long leading_zero = 0;
  return _BitScanReverse(&leading_zero, value) == 0 ? 0 : (31 - leading_zero);
}

int __inline __builtin_ctz(uint32_t value) {
  unsigned long trailing_zero = 0;
  return _BitScanForward(&trailing_zero, value) == 0 ? 32 : trailing_zero;
}

// avoid ctzl / clzl since long is 32 bits for MSVC

int __inline __builtin_ctzll(uint64_t value) {
#ifdef _M_X64
  unsigned long trailing_zero = 0;
  return _BitScanForward64(&trailing_zero, value) == 0 ? 64 : trailing_zero;
#else
  return ((value & 0xFFFFFFFF) == 0) ? (__builtin_ctz(value >> 32) + 32)
                                     : __builtin_ctz(value & 0xFFFFFFFF);
#endif
}
#endif
