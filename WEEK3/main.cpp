#include <iostream>
#include <cmath>
#include "vec3.h"
#include "color.h"
#include "ray.h"
using namespace std;

color ray_color(const ray& r) {
    Vec3 r_unit = r.direction().unit();
    auto a = 0.5*(r_unit[1] + 1.0);
    return color(1.0, 1.0, 1.0)*(1-a) + color(0.5, 0.7, 1.0)*a;
}

int main() {
    auto ratio = 16.0/9.0;
    int image_width = 400;

    int image_height = int(image_width/ratio);
    if(image_height<1) image_height = 1;

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    point3 camera_center(0,0,0);

    Vec3 viewport_u(viewport_width,0,0);
    Vec3 viewport_v(0,-viewport_height,0);

    auto pixel_delta_v = viewport_v/image_height;
    auto pixel_delta_u = viewport_u/image_width;

    auto viewport_upper_left = camera_center - Vec3(0,0,focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + pixel_delta_u*0.5 + pixel_delta_v*0.5;

    cout << "P3" << endl << image_width << " " << image_height << "\n255\n";

    for(int j=0; j<image_height; j++) {
        std::clog << "\rScanlines remaining: " << image_height-1 << ' ' << std::flush;
        for(int i=0; i<image_width; i++) {
            auto pixel_center = pixel00_loc + pixel_delta_u*i + pixel_delta_v*j;
            auto ray_direc = pixel_center - camera_center;
            ray r(camera_center,ray_direc);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}