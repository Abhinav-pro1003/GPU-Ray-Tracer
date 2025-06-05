#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
using namespace std;

class Vec3
{
private:
    double x, y, z;
public:
    Vec3(double a=0, double b=0, double c=0) {
        x=a;
        y=b;
        z=c;
    }
    Vec3 operator+(const Vec3& v) const{
        double a = x + v.x;
        double b = y + v.y;
        double c = z + v.z;
        return Vec3(a,b,c);
    }
    Vec3 operator-(const Vec3& v) const{
        double a = x - v.x;
        double b = y - v.y;
        double c = z - v.z;
        return Vec3(a,b,c);
    }
    Vec3 operator*(const Vec3& v) const{
        double a = x * v.x;
        double b = y * v.y;
        double c = z * v.z;
        return Vec3(a,b,c);
    }
    Vec3 operator/(const Vec3& v) const{
        if ( v.x==0 || v.y==0 || v.z==0) throw "Division by Zero";
        else {
            double a = x / v.x;
            double b = y / v.y;
            double c = z / v.z;
            return Vec3(a,b,c);
        }
    }
    Vec3 operator-() const {
        return Vec3(-x,-y,-z);
    }
    Vec3 operator+(const double a) const {
        return Vec3(x+a,y+a,z+a);
    }
    Vec3 operator*(const double a) const{
        return Vec3(x*a,y*a,z*a);
    }
    Vec3 operator/(const double a) const{
        return Vec3(x/a,y/a,z/a);;
    }
    const double& operator[](int a) const {
        if(a==0) return x;
        else if(a==1) return y;
        else if (a==2) return z;
        else throw "Index out of range";
    }  
    double length() const {
        return sqrt( x*x + y*y + z*z);
    }
    Vec3 unit() const {
        double a = this->length();
        return Vec3(x/a,y/a,z/a);
    }
    double dot(const Vec3& other) const {
        double a = x*other.x + y*other.y + z*other.z;
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
    Vec3 refract(const Vec3& normal, double refractive_index) const {
        Vec3 a = normal.unit();
        Vec3 b = ((a.cross(*this)).cross(a)).unit();
        Vec3 i = unit();

        double c = (a.cross(i)).length();

        double sin = c/refractive_index;
        if(sin > 1) {
            return reflect(normal);
        }
        else {
            double cos = sqrt(1-sin*sin);
            
            Vec3 r = a*cos + b*sin;
            return r;
        }
    }
};

#endif