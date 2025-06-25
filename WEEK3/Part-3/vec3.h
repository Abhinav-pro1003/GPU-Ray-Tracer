#ifndef VEC3_H
#define VEC3_H

using real_t = float;
using namespace std;

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

    vec3 reflect(const vec3& normal) const {
        vec3 a=*this-normal*dot(normal)*2;
        return a;
    }
    
    vec3 refract(const vec3& normal, double refractive_index) const {
        vec3 a = normal.unit();
        vec3 b = ((a.cross(*this)).cross(a)).unit();
        vec3 i = unit();

        real_t c = (a.cross(i)).length();

        real_t sin = c/refractive_index;
        if(sin > 1) {
            return reflect(normal);
        }
        else {
            real_t cos = sqrt(1-sin*sin);
            
            vec3 r = a*cos + b*sin;
            return r;
        }
    }
};

#endif