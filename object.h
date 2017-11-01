#pragma once
#include <glm/glm.hpp>
#include "ray.h"

class Object
{
public:
    Object() {}
    /*
     * Returns true if a ray intersects with this object, otherwise false. If
     * there are one or more intersections, set 'intersection' to the first
     * such intersection.
     */
    virtual bool intersect(Ray ray, glm::vec3& intersection) = 0;
};