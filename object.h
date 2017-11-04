#pragma once
#include "ray.h"
#include "vec.h"

class Object
{
public:
    Object() {}
    virtual ~Object() {}
    /*
     * Returns true if a ray intersects with this object, otherwise false. If
     * there are one or more intersections, set 'intersection' to the first
     * such intersection.
     */
    virtual bool intersect(Ray ray, vec3<>& intersection) = 0;
};