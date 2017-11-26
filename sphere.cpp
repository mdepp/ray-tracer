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

float Sphere::intersect(Ray ray, IntersectionData* intersectionData)
{
    // As described in https://en.wikipedia.org/wiki/Line–sphere_intersection
    auto discriminant = util::pow2(dot(ray.dir, ray.origin-m_center))
        - distance2(ray.origin, m_center) + util::pow2(m_radius);

    if (discriminant < 0) return -1; // No intersection at all

    auto term1 = -dot(ray.dir, ray.origin-m_center);
    if (discriminant == 0) // Intersects once with sphere
    {
        if (term1 >= 0)
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
        
        auto minLength = util::min(term1+term2, term1-term2);
        auto maxLength = util::max(term1+term2, term1-term2);

        if (minLength < 0)
        {
            if (maxLength < 0)
            {
                return -1; // Intersects behind ray
            }
            else
            {
                if (intersectionData)
                {
                    intersectionData->intersection = ray.origin + ray.dir*maxLength;
                    intersectionData->colour = m_colour;
                    intersectionData->normal = normalize(intersectionData->intersection - m_center);
                    intersectionData->reflectionCoefficient = m_reflectionCoefficient;
                    intersectionData->transmissionCoefficient = m_transmissionCoefficient;
                }
                return maxLength;
            }
        }
        else
        {
            if (intersectionData)
            {
                intersectionData->intersection = ray.origin + ray.dir*minLength;
                intersectionData->colour = m_colour;
                intersectionData->normal = normalize(intersectionData->intersection - m_center);
                intersectionData->reflectionCoefficient = m_reflectionCoefficient;
                intersectionData->transmissionCoefficient = m_transmissionCoefficient;
            }
            return minLength;
        }
    }
    return -1; // Should never reach this point
}