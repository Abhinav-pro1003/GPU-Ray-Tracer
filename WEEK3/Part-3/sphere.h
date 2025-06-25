#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere {
    public:
        sphere() = default;
        sphere(const point3& p, real_t r) : center(p), radius(fmax(0,r)) {}

        bool hit(const ray& r, real_t ray_tmin, real_t ray_tmax, hit_record& rec) const {

            vec3 ray_dir = r.direction();
            vec3 normal = center - r.origin();
            auto a = ray_dir.length();
            a*=a;
            
            auto h = (ray_dir.dot(normal));
            auto c = normal.length()*normal.length() - radius*radius;
            auto discriminant = h*h - a*c;

            if(discriminant<0) return false;

            auto root = (h-sycl::sqrt(discriminant))/a;

            if(root<=ray_tmin || root>=ray_tmax) {
                root = (h+sycl::sqrt(discriminant))/a;
                if(root<=ray_tmin || root>=ray_tmax) return false;
            }

            rec.t = root;
            rec.p = r.at(root);
            rec.normal = (rec.p - center)/radius;

            // rec.set_face_normal(r,rec.normal);

            return true;
        }

    private:
        point3 center;
        real_t radius;
};

#endif