/*
 * File: sphere.cpp
 * Authors: Alexander Epp (1487716) and Mitchell Epp (1498821)
 * Project: CMPUT274 Final Project
 * Description: Implementation of Sphere methods
 */

#include "sphere.h"
#include "ray.h"
#include "util.h"
#include "vec.h"

Sphere::Sphere(fvec3 center, float radius, fvec3 colour, float reflectionCoefficient, float transmissionCoefficient)
    : Object(),
      m_center(center), m_radius(radius),
      m_colour(colour),
      m_reflectionCoefficient(reflectionCoefficient),
      m_transmissionCoefficient(transmissionCoefficient)
{
}

/*
 * Returns the least number greater than epsilon among 'first' and 'second'. If both
 * are are less than epsilon, returns a negative result (but not necessarily either number).
 */
static float findLeastGreaterThan(float first, float second, float epsilon)
{
    if (first < second) return findLeastGreaterThan(second, first, epsilon);
    
    // Now first >= second
    if (first < epsilon) // Neither first nor second are greater
    {
        return -1;
    }
    else if (second >= epsilon) // Both first and second are greater
    {
        return second;
    }
    else // Second is less but not first
    {
        return first;
    }
}

float Sphere::intersect(Ray ray, IntersectionData* intersectionData, float epsilon)
{
    // As described in https://en.wikipedia.org/wiki/Line–sphere_intersection
    auto discriminant = util::pow2(dot(ray.dir, ray.origin-m_center))
        - distance2(ray.origin, m_center) + util::pow2(m_radius);

    if (discriminant < 0) return -1; // No intersection at all

    auto term1 = -dot(ray.dir, ray.origin-m_center);
    if (discriminant == 0) // Intersects once with sphere
    {
        if (term1 >= epsilon) // Intersects in from of ray
        {
            if (intersectionData)
            {
                intersectionData->intersection = ray.origin + ray.dir*term1;
                intersectionData->colour = m_colour;
                intersectionData->normal = normalize(intersectionData->intersection - m_center);
                intersectionData->reflectionCoefficient = m_reflectionCoefficient;
                intersectionData->transmissionCoefficient = m_transmissionCoefficient;
            }
            return term1;
        }
        else
        {
            return -1.f; // Intersects behind ray
        }
    }
    else // Intersects twice with sphere, so find first intersection
    {
        auto term2 = sqrt(discriminant);

        auto length = findLeastGreaterThan(term1+term2, term1-term2, epsilon);
        if (length < 0) // Both intersections are behind ray
        {
            return -1;
        }
        else // At least one intersection is in front of ray
        {
            if (intersectionData)
            {
                intersectionData->intersection = ray.origin + ray.dir*length;
                intersectionData->colour = m_colour;
                intersectionData->normal = normalize(intersectionData->intersection - m_center);
                intersectionData->reflectionCoefficient = m_reflectionCoefficient;
                intersectionData->transmissionCoefficient = m_transmissionCoefficient;
            }
            return length;
        }
    }
    return -1; // Should never reach this point
}