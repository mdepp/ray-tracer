#pragma once
#include "vec.h"
/*
 * Store a ray as an origin and direction (expected to be a unit vector)
 */
struct Ray
{
    Ray(glm::vec3 o, glm::vec3 d)
        : origin(o), dir(d)
    {}
    vec3 origin;
    vec3 dir;
};