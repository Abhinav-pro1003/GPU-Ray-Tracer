#ifndef VEC3_H
#define VEC3_H

class Vec3 {

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
            return Vec3(x/a,y/a,z/a);
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

        double length_squared() const {
            return (x*x + y*y + z*z);
        }

        Vec3 unit() const {
            return *this / this->length();
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

        bool near_zero() const {
            // Return true if the vector is close to zero in all dimensions.
            auto s = 1e-8;
            return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
        }

        static Vec3 random() {
            return Vec3(random_double(), random_double(), random_double());
        }

        static Vec3 random(double min, double max) {
            return Vec3(random_double(min,max), random_double(min,max), random_double(min,max));
        }

};

inline Vec3 random_unit_vector() {
    while (true) {
        auto p = Vec3::random(-1,1);
        auto lensq = p.length();
        lensq*=lensq;
        if (1e-160 < lensq && lensq <= 1)
            return p / sqrt(lensq);
    }
}

inline Vec3 random_on_hemisphere(const Vec3& normal) {
    Vec3 on_unit_sphere = random_unit_vector();
    if (on_unit_sphere.dot(normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - n*v.dot(n)*2;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(n.dot(-uv), 1.0);
    Vec3 r_out_perp = (uv + n*cos_theta) * etai_over_etat;
    Vec3 r_out_parallel = n * (-sqrt(fabs(1.0 - r_out_perp.length_squared())));
    return r_out_perp + r_out_parallel;
}

// inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
//     uv.print();
//     n.print();
//     cout << n.length() << " " << uv.length();
//     auto cos_theta = std::fmin(n.dot((-uv)), 1.0);
//     cout << cos_theta << endl;
//     Vec3 r_out_perp =  (uv + n*cos_theta) * etai_over_etat;
//     Vec3 r_out_parallel = n * (-std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())));
//     return r_out_perp + r_out_parallel;
// }


#endif