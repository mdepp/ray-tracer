/*
 * File: object.h
 * Authors: Alexander Epp, Mitchell Epp
 * Project: CMPUT274 Final Project
 * Description: Contains an interface used by all objects to be rendered by
 *              renderer. This guarantees each object knows about ray intersection
 *              with itself.
 */

#pragma once
#include "ray.h"
#include "vec.h"

/*
 * This defines properties that an object has at the point of any ray
 * intersection. These are:
 *     intersection: the actual point of intersection (on the object's surface)
 *     colour: the colour of the object at the intersection point
 *     normal: the surface normal of the object at the intersection point
 *     reflectionCoefficient: fraction of light that is reflected off of the
 *                            object at intersection. The rest of the light on
 *                            object at intersection.
 *     transmissionCoefficient: The fraction of light that is transmitted
 *                              through the object (is refracted upon
 *                              intersection). The remaining light on this
 *                              point of the object, without reflection or
 *                              transmission, comes from diffuse lighting.
 *     refractiveIndex: The index of refraction of object at the intersection
 *                      point.
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
     * Returns the (smallest) distance along the ray at which the ray intersects
     * with this object (this distance is negative if there is no intersection).
     * If there is an intersection, and 'intersectionData' is not nullptr,
     * populate 'intersectionData' with the attributes of the first such
     * intersection. Intersections that are within 'epsilon' of the origin of the
     * ray are considered to be the result of floating point errors, and are thus
     * ignored.
     */
    virtual float intersect(Ray ray, IntersectionData* intersectionData, float epsilon) = 0;
};