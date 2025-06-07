#ifndef HITTABLE_H
#define HITTABLE_H

class hit_record {
    public:
        point3 p;
        Vec3 normal;
        double t;

        // void set_face_normal(const ray& r, const Vec3& outward_normal) {
        //     if(outward_normal.dot(r.direction())<0) normal = -outward_normal;
        // }
};

class hittable {
    public:
        virtual ~hittable() = default;
        
        virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};
#endif