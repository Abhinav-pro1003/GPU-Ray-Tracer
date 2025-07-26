#ifndef VEC3_H
#define VEC3_H

using real_t = double;
using namespace std;

#include <random>

class vec3
{
public:
    union {
        real_t data[3];
        struct {
            real_t x, y, z;
        };
        struct {
        real_t r, g, b;
        };
    };

    vec3(real_t a = 0, real_t b = 0, real_t c = 0) : x(a), y(b), z(c) {}

    vec3 operator+(const vec3& v) const{
        real_t a = x + v.x;
        real_t b = y + v.y;
        real_t c = z + v.z;
        return vec3(a,b,c);
    }

    vec3 operator-(const vec3& v) const{
        real_t a = x - v.x;
        real_t b = y - v.y;
        real_t c = z - v.z;
        return vec3(a,b,c);
    }

    vec3 operator*(const vec3& v) const{
        real_t a = x * v.x;
        real_t b = y * v.y;
        real_t c = z * v.z;
        return vec3(a,b,c);
    }

    vec3 operator/(const vec3& v) const{
        if ( v.x==0 || v.y==0 || v.z==0) throw "Division by Zero";
        else {
            real_t a = x / v.x;
            real_t b = y / v.y;
            real_t c = z / v.z;
            return vec3(a,b,c);
        }
    }

    vec3 operator-() const {
        return vec3(-x,-y,-z);
    }

    vec3 operator+(const real_t a) const {
        return vec3(x+a,y+a,z+a);
    }

    vec3 operator*(const real_t a) const{
        return vec3(x*a,y*a,z*a);
    }

    vec3 operator/(const real_t a) const{
        return vec3(x/a,y/a,z/a);;
    }

    const real_t& operator[](int a) const {
        if(a==0) return x;
        else if(a==1) return y;
        else return z;
    } 

    real_t length() const {
        return sqrt( x*x + y*y + z*z);
    }

    real_t length_squared() const {
        return (x*x + y*y + z*z);
    }

    vec3 unit() const {
        real_t a = this->length();
        return vec3(x/a,y/a,z/a);
    }

    real_t dot(const vec3& other) const {
        real_t a = x*other.x + y*other.y + z*other.z;
        return a;
    }

    vec3 cross(const vec3& other) const {
        return vec3( y*other.z-z*other.y, z*other.x-x*other.z, x*other.y-y*other.x );
    }

    void print() const {
        cout << "(" << x << "," << y << "," << z << ")";
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
    }
    
};

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - n*v.dot(n)*2;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(n.dot(-uv), 1.0);
    vec3 r_out_perp = (uv + n*cos_theta) * etai_over_etat;
    vec3 r_out_parallel = n * (-sqrt(fabs(1.0 - r_out_perp.length_squared())));
    return r_out_perp + r_out_parallel;
}

template <class rng_t>
vec3 random_in_unit_sphere(rng_t rng, xorwow_state_t* local_rand_state) {
    auto randf = [local_rand_state, rng]() {  return rand_uniform(rng, local_rand_state); };

     while (true) {
            auto p = vec3(randf(), randf(), randf()); // in [-1,1]^3
            auto l = p.length_squared();
            if (l <= 1.0f && l>1e-30f) // inside the unit sphere
                return p / sycl::sqrt(l);
     }
}

#endif