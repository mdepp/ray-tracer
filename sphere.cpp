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

Sphere::Sphere(fvec3 center, float radius, fvec3 colour, float reflectionCoefficient)
    : Object(),
      m_center(center), m_radius(radius),
      m_colour(colour), m_reflectionCoefficient(reflectionCoefficient)
{
}

/*
 * Returns the least non-negative number among 'first' and 'second'. If both
 * are negative, returns a negative result (but not necessarily either number).
 */
static float findLeastNonNegative(float first, float second)
{
    if (first < second) return findLeastNonNegative(second, first);
    
    // Now first >= second
    if (first < 0) // Neither first nor second are non-negative
    {
        // Could return first or second, but this should lead to less rounding errors
        return -1;
    }
    else if (second >= 0) // Both first and second are non-negative
    {
        return second;
    }
    else // Second is negative but not first
    {
        return first;
    }
}

float Sphere::intersect(Ray ray, IntersectionData* intersectionData)
{
    // As described in https://en.wikipedia.org/wiki/Line–sphere_intersection
    auto discriminant = util::pow2(dot(ray.dir, ray.origin-m_center))
        - distance2(ray.origin, m_center) + util::pow2(m_radius);

    if (discriminant < 0) return -1; // No intersection at all

    auto term1 = -dot(ray.dir, ray.origin-m_center);
    if (discriminant == 0) // Intersects once with sphere
    {
        if (term1 >= 0) // Intersects in from of ray
        {
            if (intersectionData)
            {
                intersectionData->intersection = ray.origin + ray.dir*term1;
                intersectionData->colour = m_colour;
                intersectionData->normal = normalize(intersectionData->intersection - m_center);
                intersectionData->reflectionCoefficient = m_reflectionCoefficient;
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

        auto length = findLeastNonNegative(term1+term2, term1-term2);
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
            }
            return length;
        }
    }
    return -1; // Should never reach this point
}