#include <iostream>
#include <cmath>
#include "vec3.h"
#include "color.h"
using namespace std;

int main() {
    int image_width = 256;
    int image_height = 256;

    cout << "P3" << endl << image_width << " " << image_height << "\n255\n";

    for(int j=image_width-1; j>=0; --j) {
        std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i=0; i<image_height; i++) {
            auto pixel_color = color(double(i)/(image_height-1), 0, double(j)/(image_width-1));
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}