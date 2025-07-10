#ifndef RAY_H
#define RAY_H

class ray {
    private:
        point3 ori;
        vec3 dir;
    public:
        ray() {}

        ray(const point3& origin, const vec3& direction) {
            ori = origin;
            dir = direction;
        }
        const point3& origin() const{
            return ori;
        }
        const vec3& direction() const{
            return dir;
        }
        point3 at(real_t t) const {
            return ori + dir*t;
        }
};

#endif