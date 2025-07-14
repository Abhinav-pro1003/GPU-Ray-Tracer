#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable<sphere> {
    public:
        sphere() = default;
        /**
        * Constructor for a sphere object with lambertian material
        **/
        sphere(const vec3& cen, real_t r, material_t mat_type, const vec3& color) :
            center(cen), radius(r), material_type(mat_type), albedo(color)
            { }

        /**
        * Constructor for a sphere object with metal material
        **/
        sphere(const vec3& cen, real_t r, material_t mat_type, const vec3& mat_color, real_t f) :
            center(cen), radius(r), material_type(mat_type), albedo(mat_color), fuzz()
            { }

        /**
        * Constructor for a sphere object with dielectric material
        **/
        sphere(const vec3& cen, real_t r, material_t mat_type, real_t ref_idx) :
            center(cen), radius(r), material_type(mat_type), refraction_index(ref_idx)
            { }

        bool hit(const ray& r, real_t ray_tmin, real_t ray_tmax, hit_record& rec) const {

            vec3 ray_dir = r.direction();
            vec3 normal = center - r.origin();
            auto a = ray_dir.length();
            a*=a;
            
            auto h = (ray_dir.dot(normal));
            auto c = normal.length()*normal.length() - radius*radius;
            auto discriminant = h*h - a*c;

            if(discriminant<0) return false;

            auto root = (h-sqrt(discriminant))/a;

            if(root<=ray_tmin || root>=ray_tmax) {
                root = (h+sqrt(discriminant))/a;
                if(root<=ray_tmin || root>=ray_tmax) return false;
            }

            rec.t = root;
            rec.p = r.at(root);
            rec.normal = (rec.p - center)/radius;

            rec.set_face_normal(r,rec.normal);
            rec.material_type = material_type;
            
            if(material_type!=material_t::Dielectric) rec.albedo = albedo;
            if(material_type==material_t::Metal) rec.fuzz = fuzz;
            if(material_type==material_t::Dielectric) rec.refraction_index = refraction_index;

            return true;
        }

    private:
        point3 center;
        real_t radius;
        material_t material_type;
        vec3 albedo;
        real_t fuzz;
        real_t refraction_index;
};

#endif