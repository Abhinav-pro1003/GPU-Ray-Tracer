#include <sycl/sycl.hpp>
#include <vector>
#include "rtweekend.h"
#include "hittable.h"
#include "sphere.h"
#include "camera.h"
#include "render_kernel.h"


int main() {

  constexpr auto num_pixels = width * height;

  constexpr auto num_spheres = 2;

  // select the SYCL accelerator (i.e Intel GPU for this machine)
  auto queue = sycl::queue(sycl::cpu_selector_v);

  // allocate the frame buffer on the Host
  std::vector<vec3> fb(num_pixels); // frame buffer

  std::vector<sphere> spheres;
  spheres.push_back(sphere(vec3(0.0, 0.0, -1.0), 0.5));      // (small) center sphere
  spheres.push_back(sphere(vec3(0.0, -100.5, -1.0), 100.0)); // (large) ground sphere

  std::vector<xorwow_state_t> rand_states(num_pixels);
  render_init<width, height>(queue, rand_states.data());

  vec3 look_from(0.0f, 0.0f, 0.0f);
  vec3 look_at(0.0f, 0.0f, -1.0f);
  vec3 up(0.0f, 1.0f, 0.0f);
  float angle = 90.0f;
  camera cam(look_from, look_at, up, angle, static_cast<real_t>(width) / static_cast<real_t>(height));

  render<width, height, samples, num_spheres>(queue, fb.data(), spheres.data(), &cam, rand_states.data());

  // save the pixel data as an image file
  save_image<width, height>(fb.data());

  return 0;
}