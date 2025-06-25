#ifndef RENDER_KERNEL_H
#define RENDER_KERNEL_H

#include <sycl/sycl.hpp>

namespace constants {
  static constexpr auto TileX = 8;
  static constexpr auto TileY = 8;
}

template <int width, int height,int num_spheres>
class render_kernel {
 public:

  render_kernel(sycl::accessor<vec3, 1, sycl::access::mode::write, sycl::access::target::device> frame_ptr, 
    sycl::accessor<sphere, 1, sycl::access::mode::write, sycl::access::target::device> spheres_ptr) : m_frame_ptr(frame_ptr),m_spheres_ptr(spheres_ptr) {
    /* initialize accessors */
  }

  ray get_ray(real_t u, real_t v) const {

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(width)/height);

    point3 camera_center(0,0,0);

    vec3 viewport_u(viewport_width,0,0);
    vec3 viewport_v(0,-viewport_height,0);

    auto pixel_delta_v = viewport_v/height;
    auto pixel_delta_u = viewport_u/width;

    auto viewport_upper_left = camera_center - vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + pixel_delta_u*0.5 + pixel_delta_v*0.5;

    auto pixel_center = pixel00_loc + viewport_u*u + viewport_v*v;
    auto ray_direc = pixel_center - camera_center;
    return ray(camera_center,ray_direc);

  }

  void operator()(sycl::nd_item<2> item) const {

    // get our Ids
    const auto x_coord = item.get_global_id(0);
    const auto y_coord = item.get_global_id(1);

    // map the 2D indices to a single linear, 1D index
    const auto pixel_index = y_coord * width + x_coord;

    vec3 final_color;

    /* kernel logic - raytracing algorithm */

    real_t u = static_cast<real_t>(x_coord) / width;
    real_t v = static_cast<real_t>(y_coord) / height;

    auto r = get_ray(u, v);

    final_color = color(r, m_spheres_ptr);

    // write final color to the frame buffer global memory
    m_frame_ptr[pixel_index] = final_color;
  }

 private:

  /* accessor objects */
  sycl::accessor<vec3, 1, sycl::access::mode::write, sycl::access::target::device> m_frame_ptr;
  sycl::accessor<sphere, 1, sycl::access::mode::write, sycl::access::target::device> m_spheres_ptr;

  bool hit_world(const ray& r, real_t min, real_t max, hit_record& rec, const sycl::global_ptr<sphere>& spheres) const {

    auto temp_rec = hit_record{};
    auto hit_anything = false;
    auto closest_so_far = max;

    for (auto i = 0; i < num_spheres; i++) {

      if (spheres[i].hit(r, min, closest_so_far, temp_rec)) {
        hit_anything = true;
        closest_so_far = temp_rec.t;
        rec = temp_rec;
      }

    }

    return hit_anything;

  }

  vec3 color(const ray& r, const sycl::global_ptr<sphere>& spheres) const {

    hit_record rec;

    if (hit_world(r, real_t{0.001}, infinity, rec, spheres)) {
      return vec3(rec.normal[0] + 1.0, rec.normal[1] + 1.0,
      rec.normal[2] + 1.0) * 0.5;
    }

    vec3 unit_direction = r.direction().unit();
    float hit_pt = 0.5 * (unit_direction[1] + 1.0);

    return vec3(1.0, 1.0, 1.0) * (1.0 - hit_pt) + vec3(0.5, 0.7, 1.0) * hit_pt;
  }

};

template <int width, int height,int num_spheres>
void render(vec3* fb_data, const sphere* spheres) {

  constexpr auto num_pixels = width * height;

  auto frame_buf = sycl::buffer<vec3, 1>(fb_data, sycl::range<1>(num_pixels));
  sycl::buffer<sphere, 1> spheres_buf(spheres, sycl::range<1>(num_spheres));

  sycl::queue queue;

  // submit command group on device
  queue.submit([&](sycl::handler& cgh) {

    // get memory access
    auto frame_ptr = frame_buf.get_access<sycl::access::mode::write>(cgh);
    auto spheres_ptr = spheres_buf.get_access<sycl::access::mode::write>(cgh);

    // setup kernel index space
    const auto global = sycl::range<2>(width, height);
    const auto local = sycl::range<2>(constants::TileX, constants::TileY);
    const auto index_space = sycl::nd_range<2>(global, local);

    // construct kernel functor
    auto render_kernel_func = render_kernel<width, height, num_spheres>(frame_ptr, spheres_ptr);
    cgh.parallel_for(sycl::nd_range<2>(global, local), render_kernel_func);
  });
}

template <int width, int height>
void save_image(vec3* fb_data) {

  std::cout << "P3\n" << width << " " << height << "\n255\n";

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      auto pixel_index = y * width + x;
      
      int r = static_cast<int>(255.99 * fb_data[pixel_index].r);
      int g = static_cast<int>(255.99 * fb_data[pixel_index].g);
      int b = static_cast<int>(255.99 * fb_data[pixel_index].b);

      std::cout << r << " " << g << " " << b << "\n";
    }
  }
}

#endif