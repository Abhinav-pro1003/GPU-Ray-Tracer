#ifndef SPHERE_H
#define SPERE_H

#include "hittable.h"

class sphere : public hittable {
    public:
        sphere(const point3& p, double r, shared_ptr<material> mat) : center(p), radius(fmax(0,r)), mat(mat) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {

            Vec3 ray_dir = r.direction();
            Vec3 normal = center - r.origin();
            double a = ray_dir.length_squared();
            
            double h = (ray_dir.dot(normal));
            double c = normal.length_squared() - radius*radius;
            double discriminant = h*h - a*c;

            if(discriminant<0) return false;

            double root = (h-sqrt(discriminant))/a;

            if(!(ray_t.surrounds(root))) {
                root = (h+sqrt(discriminant))/a;
                if(!(ray_t.surrounds(root))) {
                    return false;
                }
            }
            rec.t = root;
            rec.p = r.at(root);
            rec.normal = (rec.p - center)/radius;
            rec.mat = mat;
            rec.set_face_normal(r,rec.normal);

            return true;
        }

    private:
        point3 center;
        double radius;
        shared_ptr<material> mat;
};

#endif
