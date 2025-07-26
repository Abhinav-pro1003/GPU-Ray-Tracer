#include <sycl/sycl.hpp>
#include <vector>

#include "rtweekend.h"
#include "hittable.h"
#include "sphere.h"
#include "camera.h"
#include "render_kernel.h"

using Hittable = hittable<sphere>;

int main() {
  constexpr auto num_hittables = 488;

  constexpr auto num_pixels = width * height;

  // select the SYCL accelerator (i.e Intel GPU for this machine)
  auto queue = sycl::queue(sycl::cpu_selector_v);

  // allocate the frame buffer on the Host
  std::vector<vec3> fb(num_pixels); // frame buffer

  std::vector<sphere> spheres;

  // define material types
  auto lambertian = material_t::Lambertian;
  auto metal = material_t::Metal;
  auto dielectric = material_t::Dielectric;

  // lambda to define the logic for random world generation
  auto create_world = [&spheres, lambertian, metal, dielectric]() {
    spheres.push_back(sphere(vec3(0.0f, -1000.0f, -1.0f), 1000.0f, lambertian, vec3(0.5f, 0.5f, 0.5f)));

    for (auto i = -11; i < 11; i++) {
      for (auto j = -11; j < 11; j++) {

        const auto choose_mat = xorand();
        vec3 center(i + xorand(), 0.2f, j + xorand());

        if (choose_mat < 0.8f) {  /// chosen lambertian
          spheres.push_back(sphere(center, 0.2, lambertian,vec3(xorand(), xorand(), xorand())));
        } 
        else if (choose_mat < 0.95f) {  /// chosen metal
          spheres.push_back(sphere(center, 0.2f, metal, vec3(0.5f * (1.0f + xorand()),0.5f * (1.0f + xorand()), 0.5f * (1.0f + xorand())), 0.5f * xorand()));
        } 
        else {  /// chosen dielectric
          spheres.push_back(sphere(center, 0.2f, dielectric, 1.5f));
        }
      }
    }

    spheres.push_back(sphere(vec3(0.0, 1.0, 0.0), 1.0, dielectric, 1.5));
    spheres.push_back(sphere(vec3(-4.0, 1.0, 0.0), 1.0, lambertian, vec3(0.4, 0.2, 0.1)));
    spheres.push_back(sphere(vec3(4.0, 1.0, 0.0), 1.0, metal, vec3(0.7f, 0.6, 0.5), 0.0));
    
  };

  create_world();

  std::vector<xorwow_state_t> rand_states(num_pixels);
  render_init<width, height>(queue, rand_states.data());

  vec3 look_from(13.0f, 2.0f, 3.0f);
  vec3 look_at(0.0f, 0.0f, 0.0f);
  vec3 up(0.0f, 1.0f, 0.0f);
  float angle = 20.0f;
  camera cam(look_from, look_at, up, angle, static_cast<real_t>(width) / static_cast<real_t>(height));

  render<width, height, samples, num_hittables>(queue, fb.data(), spheres.data(), &cam, rand_states.data());

  // save the pixel data as an image file
  save_image<width, height>(fb.data());

  return 0;
}