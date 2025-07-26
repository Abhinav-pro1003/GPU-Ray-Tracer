class camera {
  public:
    vec3 center;
    vec3 pixel00_loc;
    vec3 viewport_u;
    vec3 viewport_v;

    camera(vec3 look_from, vec3 look_at, vec3 up, float angle, real_t ratio) {

        center = look_from;
        auto focal_length = (look_from - look_at).length();
        auto theta = degrees_to_radians(angle);
        auto h = std::tan(theta / 2);
        
        auto viewport_height = 2 * h * focal_length;
        auto viewport_width = viewport_height * ratio;

        vec3 w = (look_from - look_at).unit();
        vec3 u = up.cross(w);
        vec3 v = w.cross(u);

        viewport_u = u * viewport_width;
        viewport_v = (-v) * viewport_height;

        pixel00_loc = center - w * focal_length - viewport_u / 2 - viewport_v / 2 + (viewport_u/width + viewport_v/height) * 0.5;
    }

    template <class rng_t>
    ray get_ray(real_t u, real_t v, rng_t rng, xorwow_state_t* local_rand_state) const {
        vec3 pixel_sample = pixel00_loc + viewport_u * u + viewport_v * v;
        return ray(center, pixel_sample - center);
    }
};