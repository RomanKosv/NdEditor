#ifndef TRIANGLES_H
#define TRIANGLES_H


template<typename T>
struct triple{
    T a, b, c;
};

typedef triple<float> vec3;

typedef triple<vec3> triangle;

#endif // TRIANGLES_H
