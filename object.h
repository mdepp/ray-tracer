#pragma once
#include "ray.h"
#include "vec.h"

/*
 * Stores all data of a single ray-object intersection. 'reflectionCoefficient' is the fraction of incoming light that is reflected from
 * the object (specular light), 'transmissionCoefficient' is the fraction of light that is refracted through the object
 */
struct IntersectionData
{
    fvec3 intersection;
    fvec3 colour;
    fvec3 normal;
    float reflectionCoefficient;
    float transmissionCoefficient;
    float refractiveIndex;
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