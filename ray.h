#pragma once
#include "vec.h"
/*
 * Store a ray as an origin and direction (expected to be a unit vector)
 */
struct Ray
{
    Ray(vec3<> o, vec3<> d)
        : origin(o), dir(d)
    {}
    vec3<> origin;
    vec3<> dir;
};