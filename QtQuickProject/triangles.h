#ifndef TRIANGLES_H
#define TRIANGLES_H

#include <cstdint>

template<typename T>
struct triple{
    T a, b, c;
};

struct RGBA{
    float r = 0, g = 0, b = 0, a = 1;
};

struct Point{
    float x, y, z;
    RGBA rgba;
};

typedef Point vec3;

typedef triple<vec3> triangle;

#endif // TRIANGLES_H
