#ifndef XOROSHIRO256PLUSPLUS_H
#define XOROSHIRO256PLUSPLUS_H

/* Modified by D. Lemire, August 2017 */
#include "splitmix64.h"
#include <stdint.h>

// original documentation by Vigna:
/* This is xoshiro256++ 1.0, one of our all-purpose, rock-solid generators.
   It has excellent (sub-ns) speed, a state (256 bits) that is large
   enough for any parallel application, and it passes all tests we are
   aware of.

   For generating just floating-point numbers, xoshiro256+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

// state for xoroshiro256plusplus
uint64_t xoroshiro256plusplus_s[4];

static inline uint64_t rotl(const uint64_t x, int k) {
  return (x << k) | (x >> (64 - k));
}

// call this one before calling xoroshiro256plusplus
static inline void xoroshiro256plusplus_seed(uint64_t seed) {
  xoroshiro256plusplus_s[0] = splitmix64_r(&seed);
  xoroshiro256plusplus_s[1] = splitmix64_r(&seed);
  xoroshiro256plusplus_s[2] = splitmix64_r(&seed);
  xoroshiro256plusplus_s[3] = splitmix64_r(&seed);
}

// returns random number, modifies xoroshiro256plusplus_s
static inline uint64_t xoroshiro256plusplus(void) {
  const uint64_t s0 = xoroshiro256plusplus_s[0];
  const uint64_t s1 = xoroshiro256plusplus_s[1];
  uint64_t s2 = xoroshiro256plusplus_s[2];
  uint64_t s3 = xoroshiro256plusplus_s[3];
  const uint64_t result = rotl(s0 + s3, 23) + s0;
  const uint64_t t = s1 << 17;

  s2 ^= s0;
  s3 ^= s1;

  xoroshiro256plusplus_s[0] ^= s3;
  xoroshiro256plusplus_s[1] ^= s2;
  xoroshiro256plusplus_s[2] ^= t;
  xoroshiro256plusplus_s[3] = rotl(s3, 45);

  return result;
}

// Converts a given uint64_t value `x` into a 64-bit floating
// point value in the range of [0.0, 1.0)
static inline double __xoroshiro256plusplus_d(const uint64_t x) {
  return (x >> 11) * 0x1.0p-53;
}

// returns random number, modifies xoroshiro256plusplus_s
static inline double xoroshiro256plusplus_d(void) {
  const uint64_t x = xoroshiro256plusplus();

  return __xoroshiro256plusplus_d(x);
}

#endif // XOROSHIRO256PLUSPLUS_H
