#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#define hash(x) (mueller_hash(x))
#include <iostream>
#include <memory>
#include <cmath>

using real_t = float;
using int_type = uint32_t;
using namespace std;
using std::make_shared;
using std::shared_ptr;

constexpr auto width = 400;
constexpr auto height = 225;
constexpr auto samples = 10;
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

template <class rng_t, class state_t, typename data_t = float>
inline data_t rand_uniform(rng_t rng, state_t* state) {
  auto a = rng(state) >> 9;
  auto res = data_t{0.0};
  *(reinterpret_cast<int_type*>(&res)) = a | 0x3F800000;
  return res - data_t{1.0};
}

#include "vec3.h"
using color = vec3;
using point3 = vec3;
#include "ray.h"

#endif