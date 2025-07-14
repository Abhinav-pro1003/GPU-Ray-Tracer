#ifndef RENDER_KERNEL_H
#define RENDER_KERNEL_H

#include <sycl/sycl.hpp>

namespace constants {
  static constexpr auto TileX = 8;
  static constexpr auto TileY = 8;
}

template <int width, int height>
class render_init_kernel {
  template <typename data_t>
  using write_accessor_t = sycl::accessor<data_t, 1, sycl::access::mode::write,
                                          sycl::access::target::global_buffer>;

 public:
  render_init_kernel(sycl::accessor<xorwow_state_t, 1, sycl::access::mode::write, sycl::access::target::device> rand_states_ptr)
      : m_rand_states_ptr(rand_states_ptr) {}

  void operator()(sycl::nd_item<2> item) const {
    const auto x_coord = item.get_global_id(0);
    const auto y_coord = item.get_global_id(1);

    const auto pixel_index = y_coord * width + x_coord;

    const auto state = get_initial_xorwow_state(pixel_index);
    m_rand_states_ptr[pixel_index] = state;
  }

 private:
  sycl::accessor<xorwow_state_t, 1, sycl::access::mode::write, sycl::access::target::device> m_rand_states_ptr;
};

template <int width, int height,int samples, int num_hittables, class Hittable>
class render_kernel {
 public:

  render_kernel(sycl::accessor<vec3, 1, sycl::access::mode::write, sycl::access::target::device> frame_ptr, 
    sycl::accessor<Hittable, 1, sycl::access::mode::read_write, sycl::access::target::device> hittables_ptr,
    sycl::accessor<camera, 1, sycl::access::mode::read, sycl::access::target::device> camera_ptr,
    sycl::accessor<xorwow_state_t, 1, sycl::access::mode::read_write, sycl::access::target::device> rand_states_ptr) : m_frame_ptr(frame_ptr),m_hittables_ptr(hittables_ptr),m_camera_ptr(camera_ptr),m_rand_states_ptr(rand_states_ptr) {
    /* initialize accessors */
  }

  void operator()(sycl::nd_item<2> item) const {

    // get our Ids
    const auto x_coord = item.get_global_id(0);
    const auto y_coord = item.get_global_id(1);

    // map the 2D indices to a single linear, 1D index
    const auto pixel_index = y_coord * width + x_coord;

    // initialize local (for the current thread) random state
    auto local_rand_state = m_rand_states_ptr[pixel_index];

    // create a 'rng' function object using a lambda
    auto rng = [](xorwow_state_t* state) { return xorwow(state); };

    // capture the rand generator state -> return a uniform value
    auto randf = [&local_rand_state, rng]() {  return rand_uniform(rng, &local_rand_state); };

    // color sampling
    vec3 final_color(0.0, 0.0, 0.0);
    for (auto i = 0; i < samples; i++) {
      const auto u = (x_coord + randf()) / static_cast<real_t>(width);
      const auto v = (y_coord + randf()) / static_cast<real_t>(height);

      ray r = m_camera_ptr[0].get_ray(u, v, rng, &local_rand_state);
      
      final_color = final_color + color(r, m_hittables_ptr, rng, &local_rand_state, max_depth);
    }
    final_color = final_color/static_cast<real_t>(samples);

    // apply gamma correction
    final_color.x = sycl::sqrt(final_color.x);
    final_color.y = sycl::sqrt(final_color.y);
    final_color.z = sycl::sqrt(final_color.z);

    // store final color
    m_frame_ptr[pixel_index] = final_color;

  }

 private:

  /* accessor objects */
  sycl::accessor<vec3, 1, sycl::access::mode::write, sycl::access::target::device> m_frame_ptr;
  sycl::accessor<Hittable, 1, sycl::access::mode::read_write, sycl::access::target::device> m_hittables_ptr;
  sycl::accessor<camera, 1, sycl::access::mode::read, sycl::access::target::device> m_camera_ptr;
  sycl::accessor<xorwow_state_t, 1, sycl::access::mode::read_write, sycl::access::target::device> m_rand_states_ptr;

  bool hit_world(const ray& r, real_t min, real_t max, hit_record& rec, const sycl::global_ptr<Hittable>& hittables) const {

    auto temp_rec = hit_record{};
    auto hit_anything = false;
    auto closest_so_far = max;

    for (auto i = 0; i < num_hittables; i++) {

      if (hittables[i].hit(r, min, closest_so_far, temp_rec)) {
        hit_anything = true;
        closest_so_far = temp_rec.t;
        rec = temp_rec;
      }

    }

    return hit_anything;

  }

  template <class rng_t>
  vec3 color(const ray& r, const sycl::global_ptr<Hittable>& hittables,rng_t rng, xorwow_state_t* local_rand_state, int depth) const {

    static const auto max_real = std::numeric_limits<real_t>::max();
    ray cur_ray = r;
    vec3 cur_attenuation(1.0, 1.0, 1.0);

    for (auto i = 0; i < depth; i++) {
      hit_record rec;

      if (hit_world(cur_ray, real_t{0.001}, infinity, rec, hittables)) {
        
        vec3 attenuation;
        ray scattered;

        if(scatter_material(cur_ray,rec,attenuation,scattered,rng,local_rand_state)) {
          cur_ray = scattered;
          cur_attenuation = cur_attenuation * attenuation;
        }
        else {
          return vec3(0,0,0);
        }
      }
      else {
        vec3 unit_direction = cur_ray.direction().unit();
        float t = (unit_direction[1] + 1.0) * 0.5;
        vec3 c = vec3(1.0, 1.0, 1.0) * (1.0 - t) + vec3(0.5, 0.7, 1.0) * t;
        return cur_attenuation * c;
      }
    }
    // exceeded recursion ...
    return vec3(0, 0, 0);
  }

};

template <int width, int height>
void render_init(sycl::queue& queue, xorwow_state_t* rand_states) {

  constexpr auto num_pixels = width * height;

  // allocate memory on device
  auto rand_states_buf = sycl::buffer<xorwow_state_t, 1>(rand_states, sycl::range<1>(num_pixels));

  // submit command group on device
  queue.submit([&](sycl::handler& cgh) {

    // get memory access
    auto rand_states_ptr = rand_states_buf.get_access<sycl::access::mode::write>(cgh);

    // setup kernel index space
    const auto global = sycl::range<2>(width, height);
    const auto local = sycl::range<2>(constants::TileX, constants::TileY);
    const auto index_space = sycl::nd_range<2>(sycl::range<2>(width, height), sycl::range<2>(constants::TileX, constants::TileY));

    // construct kernel functor
    auto render_init_func = render_init_kernel<width, height>(rand_states_ptr);

    // execute kernel
    cgh.parallel_for(index_space, render_init_func);
  });
}

template <int width, int height, int samples, int num_hittables, class Hittable>
void render(sycl::queue& queue, vec3* fb_data, Hittable* hittables, camera* cam, xorwow_state_t* rand_states) {

  constexpr auto num_pixels = width * height;

  // allocate memory on device
  sycl::buffer<vec3, 1> frame_buf(fb_data, sycl::range<1>(num_pixels));
  sycl::buffer<Hittable, 1> hittables_buf(hittables, sycl::range<1>(num_hittables));
  sycl::buffer<camera, 1> camera_buf(cam, sycl::range<1>(1));
  sycl::buffer<xorwow_state_t, 1> rand_states_buf(rand_states, sycl::range<1>(num_pixels));

  // submit command group on device
  queue.submit([&](sycl::handler& cgh) {

    // get memory access
    auto frame_ptr = frame_buf.get_access<sycl::access::mode::write>(cgh);
    auto hittables_ptr = hittables_buf.template get_access<sycl::access::mode::read_write>(cgh);
    auto camera_ptr = camera_buf.get_access<sycl::access::mode::read>(cgh);
    auto rand_states_ptr = rand_states_buf.get_access<sycl::access::mode::read_write>(cgh);

    // setup kernel index space
    const auto global = sycl::range<2>(width, height);
    const auto local = sycl::range<2>(constants::TileX, constants::TileY);
    const auto index_space = sycl::nd_range<2>(global, local);
    
    // construct kernel functor
    auto render_func = render_kernel<width, height, samples, num_hittables, Hittable>(frame_ptr, hittables_ptr, camera_ptr, rand_states_ptr);
    cgh.parallel_for(index_space, render_func);
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