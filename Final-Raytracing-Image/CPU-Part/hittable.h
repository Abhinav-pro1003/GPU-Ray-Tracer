#ifndef HITTABLE_H
#define HITTABLE_H

class material;

class hit_record {
    public:
        point3 p;
        Vec3 normal;
        shared_ptr<material> mat;
        double t;
        bool front_face;

        // // This function is for outward normal uncomment this if you prefer it.
        
        void set_face_normal(const ray& r,Vec3& outward_normal) {
            // Sets the hit record normal vector.
            // NOTE: the parameter `outward_normal` is assumed to have unit length.

            front_face = r.direction().dot(outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;
        
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};
#endif