#include <sycl/sycl.hpp>
#include <vector>
#include "rtweekend.h"
#include "hittable.h"
#include "sphere.h"
#include "render_kernel.h"


int main() {
  // frame buffer dimensions
  constexpr auto width = 400;
  constexpr auto height = 225;
  constexpr auto num_pixels = width * height;

  constexpr auto num_spheres = 2;
  // select the SYCL accelerator (i.e Intel GPU for this machine)
  auto queue = sycl::queue(sycl::cpu_selector_v);

  // allocate the frame buffer on the Host
  std::vector<vec3> fb(num_pixels); // frame buffer

  std::vector<sphere> spheres;
  spheres.push_back(sphere(vec3(0.0, 0.0, -1.0), 0.5));      // (small) center sphere
  spheres.push_back(sphere(vec3(0.0, -100.5, -1.0), 100.0)); // (large) ground sphere

  render<width, height, num_spheres>(fb.data(), spheres.data());

  // save the pixel data as an image file
  save_image<width, height>(fb.data());

  return 0;
}