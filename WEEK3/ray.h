#ifndef RAY_H
#define RAY_H

class ray {
    private:
        point3 ori;
        Vec3 dir;
    public:
        ray() {}

        ray(const point3& origin, const Vec3& direction) {
            ori = origin;
            dir = direction;
        }
        const point3& origin() const{
            return ori;
        }
        const Vec3& direction() const{
            return dir;
        }
        point3 at(double t) const {
            return ori + dir*t;
        }
};

#endif