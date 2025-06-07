#ifndef SPHERE_H
#define SPERE_H

#include "hittable.h"

class sphere : public hittable {
    public:
        sphere(const point3& p, double r) : center(p), radius(fmax(0,r)) {}

        bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
            Vec3 d = r.direction();
            Vec3 e = center - r.origin();
            double a = d.length();
            a*=a;
            double b = (d.dot(e));
            double c = e.length()*e.length() - radius*radius;
            double discriminant = b*b - a*c;

            if(discriminant<0) return false;

            double root = (b-sqrt(discriminant))/a;
            if(root<=ray_tmin || root>=ray_tmax) {
                double root = (b+sqrt(discriminant))/a;
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
        double radius;
};

#endif