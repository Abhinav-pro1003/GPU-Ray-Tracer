#include <iostream>
#include <cmath>
#include "vec3.h"
using namespace std;

int main() {
    int image_width = 256;
    int image_height = 256;

    cout << "P3" << endl << image_width << " " << image_height << "\n255\n";

    for(int j=image_width-1; j>=0; --j) {
        std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i=0; i<image_height; i++) {
            auto r = double(i)/(image_height-1);
            auto g = 0.0;
            auto b = double(j)/(image_width-1);

            int ir = int(255.99*r);
            int ig = int(255.99*g);
            int ib = int(255.99*b);

            cout << ir << " " << ig << " " << ib << endl;
        }
    }
    std::clog << "\rDone.                 \n";
}