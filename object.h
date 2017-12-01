/*
 * File: object.h
 * Authors: Alexander Epp (1487716) and Mitchell Epp (1498821)
 * Project: CMPUT274 Final Project
 * Description: Contains an interface used by all objects to be rendered by
 *              renderer. This guarantees each object knows about ray intersection
 *              with itself.
 */

#pragma once
#include "ray.h"
#include "vec.h"

struct IntersectionData
{
    fvec3 intersection;
    fvec3 colour;
    fvec3 normal;
    float reflectionCoefficient;
};

class Object
{
public:
    Object() {}
    virtual ~Object() {}
    /*
     * Returns the distance along the ray at which the ray intersects with
     * this object (this distance is negative if there is no intersection).
     * If there is an intersection, and 'intersectionData' is not nullptr,
     * populate 'intersectionData' with the attributes of the first such
     * intersection.
     */
    virtual float intersect(Ray ray, IntersectionData* intersectionData) = 0;
};