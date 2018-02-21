/*
 * File: plane.cpp
 * Authors: Alexander Epp, Mitchell Epp
 * Project: CMPUT274 Final Project
 * Description: Implementations of Plane functions.
 */

#include "plane.h"
#include "ray.h"
#include "util.h"
#include "vec.h"

Plane::Plane(fvec3 origin, fvec3 normal, fvec3 colour, float reflectionCoefficient, float transmissionCoefficient, float refractiveIndex)
  : Object(),
    m_origin(origin),
    m_normal(normalize(normal)),
    m_colour(colour),
    m_reflectionCoefficient(reflectionCoefficient),
    m_transmissionCoefficient(transmissionCoefficient),
    m_refractiveIndex(refractiveIndex)
{

}
float Plane::intersect(Ray ray, IntersectionData* intersectionData, float epsilon)
{
    // https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    auto dotProduct = dot(m_normal, ray.dir);
    if (util::abs(dotProduct) == 0) // Ray is parallel to plane
    {
        // No intersection if parallel
        return -1;
    }

    auto distance = dot(m_origin - ray.origin, m_normal) / dotProduct;
    if (distance > epsilon)
    {
        if (intersectionData)
        {
            intersectionData->intersection = ray.origin + ray.dir*distance;
            intersectionData->colour = m_colour;
            intersectionData->normal = m_normal;
            intersectionData->reflectionCoefficient = m_reflectionCoefficient;
            intersectionData->transmissionCoefficient = m_transmissionCoefficient;
            intersectionData->refractiveIndex = m_refractiveIndex;
        }
        return distance;
    }
    else
    {
        return -1;
    }
}
