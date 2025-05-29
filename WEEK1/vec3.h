#include <cmath>
#include <iostream>
using namespace std;

class Vec3
{
private:
    float x, y, z;
public:
    Vec3(float a, float b, float c) {
        x=a;
        y=b;
        z=c;
    }
    Vec3 operator+(const Vec3& v) const{
        float a = x + v.x;
        float b = y + v.y;
        float c = z + v.z;
        return Vec3(a,b,c);
    }
    Vec3 operator-(const Vec3& v) const{
        float a = x - v.x;
        float b = y - v.y;
        float c = z - v.z;
        return Vec3(a,b,c);
    }
    Vec3 operator*(const Vec3& v) const{
        float a = x * v.x;
        float b = y * v.y;
        float c = z * v.z;
        return Vec3(a,b,c);
    }
    Vec3 operator/(const Vec3& v) const{
        if ( v.x==0 || v.y==0 || v.z==0) throw "Division by Zero";
        else {
            float a = x / v.x;
            float b = y / v.y;
            float c = z / v.z;
            return Vec3(a,b,c);
        }
    }
    Vec3 operator-() const {
        return Vec3(-x,-y,-z);
    }
    Vec3 operator+(const float a) const {
        return Vec3(x+a,y+a,z+a);
    }
    Vec3 operator*(const float a) const{
        return Vec3(x*a,y*a,z*a);
    }
    Vec3 operator/(const float a) const{
        return Vec3(x/a,y/a,z/a);;
    }
    float& operator[](int a) {
        if(a==0) return x;
        else if(a==1) return y;
        else if (a==2) return z;
        else throw "Index out of range";
    }  
    float length() const {
        return sqrt( x*x + y*y + z*z);
    }
    Vec3 unit() const {
        float a = this->length();
        return Vec3(x/a,y/a,z/a);
    }
    float dot(const Vec3& other) const {
        float a = x*other.x + y*other.y + z*other.z;
        return a;
    }
    Vec3 cross(const Vec3& other) const {
        return Vec3( y*other.z-z*other.y, z*other.x-x*other.z, x*other.y-y*other.x );
    }
    void print() const {
        cout << "(" << x << "," << y << "," << z << ")";
    }
    Vec3 reflect(const Vec3& normal) const {
        Vec3 a=*this-normal*dot(normal)*2;
        return a;
    }
    Vec3 refract(const Vec3& normal, float refractive_index) const {
        Vec3 a = normal.unit();
        Vec3 b = ((a.cross(*this)).cross(a)).unit();
        Vec3 i = unit();

        float c = (a.cross(i)).length();

        float sin = c/refractive_index;
        if(sin > 1) {
            return reflect(normal);
        }
        else {
            float cos = sqrt(1-sin*sin);
            
            Vec3 r = a*cos + b*sin;
            return r;
        }
    }
};
