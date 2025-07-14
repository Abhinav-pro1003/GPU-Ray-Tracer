#ifndef HITTABLE_H
#define HITTABLE_H

class hit_record {
 public:
  hit_record() = default;

  void set_face_normal(const ray& r,vec3& outward_normal) {
    // Sets the hit record normal vector.
    // NOTE: the parameter `outward_normal` is assumed to have unit length.

    front_face = r.direction().dot(outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }

    vec3 center;
    real_t radius;
    real_t t;
    point3 p;
    vec3 normal;
    bool front_face;

    // material properties
    material_t material_type;
    vec3 albedo;
    real_t fuzz;
    real_t refraction_index;
};

template <class rng_t>
bool scatter_material(const ray& r_in, const hit_record& rec,vec3& attenuation, ray& scattered,rng_t rng, xorwow_state_t* local_rand_state) {
  switch (rec.material_type) {
    case material_t::Lambertian:
    {
      auto scatter_direction = rec.normal + random_in_unit_sphere(rng,local_rand_state);
      // evaluate the lambertian material type
      if (scatter_direction.near_zero())
          scatter_direction = rec.normal;

      scattered = ray(rec.p, scatter_direction);
      attenuation = rec.albedo;

      return true;
    }

    case material_t::Metal:
    {
      auto randf = [local_rand_state, rng]() {  return rand_uniform(rng, local_rand_state); };

      vec3 reflected = reflect(r_in.direction(), rec.normal);;
      reflected = reflected.unit() + (random_in_unit_sphere(rng,local_rand_state) * rec.fuzz);
      scattered = ray(rec.p, reflected);
      attenuation = rec.albedo;

      // evaluate the metal material type
      return ((rec.normal.dot(scattered.direction())) > 0);
    }

    case material_t::Dielectric:
    {
      auto randf = [local_rand_state, rng]() {  return rand_uniform(rng, local_rand_state); };

      attenuation = color(1.0, 1.0, 1.0);
      double ri = rec.front_face ? (1.0/rec.refraction_index) : rec.refraction_index;

      vec3 unit_direction = r_in.direction().unit();
      
      double cos_theta = fmin(rec.normal.dot(-unit_direction), 1.0);
      double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

      bool cannot_refract = ri * sin_theta > 1.0;
      vec3 direction;

      auto r0 = (1 - ri) / (1 + ri);
      r0 = r0*r0;

      if (cannot_refract || (r0 + (1-r0)*std::pow((1 - cos_theta),5)) > randf())
          direction = reflect(unit_direction, rec.normal);
      else
          direction = refract(unit_direction, rec.normal, ri);

      scattered = ray(rec.p, direction);
      // evaluate the dielectric material type
      return true;
    }

    default:
      /* cannot reach here since a material must be defined
      * when upon initialization of a geometry (sphere). */
      return false;
  }
}

template <class derived>
struct crtp {
  SYCL_EXTERNAL
  derived& underlying() { return static_cast<derived&>(*this); }

  SYCL_EXTERNAL
  const derived& underlying() const {
    return static_cast<const derived&>(*this);
  }
};

template <class geometry>
class hittable : public crtp<geometry> {
    public:
        ~hittable() = default;
        
        bool hit(const ray& r, real_t ray_tmin, real_t ray_tmax, hit_record& rec) const {
            return this->underlying().hit(r, ray_tmin, ray_tmax, rec);
        }
};
#endif