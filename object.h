#pragma once
#include "ray.h"
#include "vec.h"

struct IntersectionData
{
    fvec3 intersection;
    fvec3 colour;
    fvec3 normal;
    /*
     * Lighting calculations are split into two parts. First, diffuse lighting is calculated
     * as in the Phong model, and the result is multiplied by the diffuse coefficient. Next,
     * reflectivity is calculated by Schlick's approximation, which is used to create reflective
     * and refractive rays (oriented by Snell's law), and the result is scaled by the one minus
     * the diffuse constant (that is, the diffuse constant linearly interpolates results of each
     * calculation).
     */
    //float diffuseCoefficient;
    bool transparent;
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