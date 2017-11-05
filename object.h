#pragma once
#include "ray.h"
#include "vec.h"

class Object
{
public:
    Object() {}
    virtual ~Object() {}
    /*
     * Returns the distance along the ray at which the ray intersects with
     * this object (this distance is negative if there is no intersection).
     * If there is an intersection, and if 'intersection' is not nullptr,
     * set 'intersection' to be the point at which the first intersection occurs.
     */
    virtual float intersect(Ray ray, vec3<>* intersection) = 0;
};