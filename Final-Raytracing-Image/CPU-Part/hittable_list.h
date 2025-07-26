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

        bool hit(const ray&r, interval ray_t, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            double close = ray_t.max;

            for(const auto& object : objects) {
                if (object->hit(r, interval(ray_t.min, close), temp_rec)) {
                    hit_anything = true;
                    close = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }

};

#endif