#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#define hash(x) (mueller_hash(x))
#include <iostream>
#include <memory>
#include <cmath>

using real_t = double;
using int_type = uint32_t;
using namespace std;
using std::make_shared;
using std::shared_ptr;

constexpr auto width = 400;
constexpr auto height = 225;
constexpr auto samples = 100;
constexpr auto max_depth = 50;


const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

struct xorwow_state_t {
  // xorshift values (160 bits)
  int_type x;
  int_type y;
  int_type z;
  int_type w;
  int_type v;
  // sequence value
  int_type d;
};

inline int_type xorwow(xorwow_state_t* state) {
  int_type t = (state->x ^ (state->x >> 2));
  state->x = state->y;
  state->y = state->z;
  state->z = state->w;
  state->v = (state->v ^ (state->v << 4)) ^ (t ^ (t << 1));
  state->d = state->d + 362437;
  return state->v + state->d;
}

inline int_type mueller_hash(int_type x) {
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x) * 0x45d9f3b;
  x = ((x >> 16) ^ x);
  return x;
}

inline xorwow_state_t get_initial_xorwow_state(int_type seed) {
  xorwow_state_t state;
  state.d = !seed ? 4294967295 : seed;
  for (auto i = 0; i < 8; i++) {
    state.x = hash(state.d);
    state.y = hash(state.x);
    state.z = hash(state.y);
    state.w = hash(state.z);
    state.v = hash(state.w);
    state.d = hash(state.v);
  }
  return state;
}

template <class rng_t, class state_t>
inline double rand_uniform(rng_t rng, state_t* state) {
  // Generate two 32-bit random parts for 52 bits of precision
  uint64_t hi = rng(state);      // upper 32 bits
  uint64_t lo = rng(state);      // lower 32 bits
  uint64_t mantissa = ((hi & 0x000FFFFF) << 32) | lo; // 52 bits

  uint64_t bits = (uint64_t(0x3FF) << 52) | mantissa; // exponent = 1023 (bias), so value ∈ [1,2)
  double res;
  std::memcpy(&res, &bits, sizeof(bits)); // safely reinterpret bits
  return res - 1.0; // map to [0, 1)
}

inline double xorand() {
  uint64_t hi = rand();
  uint64_t lo = rand();
  uint64_t mantissa = ((hi & 0x000FFFFF) << 32) | lo;
  uint64_t bits = (uint64_t(0x3FF) << 52) | mantissa;
  double res;
  std::memcpy(&res, &bits, sizeof(bits));
  return res - 1.0;
}


enum class material_t { Lambertian, Metal, Dielectric };


#include "vec3.h"
using color = vec3;
using point3 = vec3;
#include "ray.h"

#endif