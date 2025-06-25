#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include "hittable.h"

class hittable_list : public hittable {
    public:
        vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> a) {add(a);}

        void add(shared_ptr<hittable> a) {
            objects.push_back(a);
        }

        void clear() {
            objects.clear();
        }

        bool hit(const ray&r, real_t ray_tmin, real_t ray_tmax, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            real_t close = ray_tmax;

            for(const auto& object : objects) {
                if (object->hit(r, ray_tmin, close, temp_rec)) {
                    hit_anything = true;
                    close = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }

};

#endif