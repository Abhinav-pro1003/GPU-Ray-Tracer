#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <iostream>
#include <memory>
#include <cmath>

using real_t = float;
using namespace std;
using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#include "vec3.h"
using color = vec3;
using point3 = vec3;
#include "ray.h"

#endif