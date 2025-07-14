#include <sycl/sycl.hpp>
#include <vector>
#include <cstdlib>
#include "rtweekend.h"
#include "hittable.h"
#include "sphere.h"
#include "camera.h"
#include "render_kernel.h"

using Hittable = hittable<sphere>;
int main() {

  constexpr auto num_hittables = 5;

  constexpr auto num_pixels = width * height;

  // select the SYCL accelerator (i.e Intel GPU for this machine)
  auto queue = sycl::queue(sycl::cpu_selector_v);

  // allocate the frame buffer on the Host
  std::vector<vec3> fb(num_pixels); // frame buffer

  std::vector<sphere> hittables;
  hittables.push_back(sphere(vec3(0.0, 0.0, -1.2), 0.5, material_t::Lambertian, vec3(0.1,0.2,0.5)));      // (small) center sphere
  hittables.push_back(sphere(vec3(0.0, -100.5, -1.0), 100.0, material_t::Lambertian, vec3(0.8,0.8,0))); // (large) ground sphere
  hittables.push_back(sphere(vec3(-1.0, 0.0, -1.0), 0.5, material_t::Dielectric, 1.5));
  hittables.push_back(sphere(vec3(-1.0, 0.0, -1.0), 0.4, material_t::Dielectric, 1.00/1.50));
  hittables.push_back(sphere(vec3( 1.0, 0.0, -1.0), 0.5, material_t::Metal, vec3(0.8, 0.6, 0.2), 0.5));

  std::vector<xorwow_state_t> rand_states(num_pixels);
  render_init<width, height>(queue, rand_states.data());

  vec3 look_from(-2.0f, 2.0f, 1.0f);
  vec3 look_at(0.0f, 0.0f, -1.0f);
  vec3 up(0.0f, 1.0f, 0.0f);
  float angle = 90.0f;
  camera cam(look_from, look_at, up, angle, static_cast<real_t>(width) / static_cast<real_t>(height));

  render<width, height, samples, num_hittables>(queue, fb.data(), hittables.data(), &cam, rand_states.data());

  // save the pixel data as an image file
  save_image<width, height>(fb.data());

  return 0;
}