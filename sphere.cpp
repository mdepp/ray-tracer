#include "sphere.h"
#include "ray.h"
#include "util.h"
#include "vec.h"

Sphere::Sphere(vec3<> center, float radius)
 : Object(), m_center(center), m_radius(radius)
{
}

float Sphere::intersect(Ray ray, vec3<>* intersection)
{
    // As described in https://en.wikipedia.org/wiki/Line–sphere_intersection
    auto discriminant = pow2(dot(ray.dir, ray.origin-m_center))
        - distance2(ray.origin, m_center) + pow2(m_radius);

    if (discriminant < 0) return -1; // No intersection at all

    auto term1 = -dot(ray.dir, ray.origin-m_center);
    if (discriminant == 0) // Intersects once with sphere
    {
        if (term1 >= 0)
        {
            if (intersection) *intersection = ray.origin + ray.dir*term1;
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
        
        auto minLength = min(term1+term2, term1-term2);
        auto maxLength = max(term1+term2, term1-term2);

        if (minLength < 0)
        {
            if (maxLength < 0)
            {
                return -1; // Intersects behind ray
            }
            else
            {
                if (intersection) *intersection = ray.origin + ray.dir*maxLength;
                return maxLength;
            }
        }
        else
        {
            if (intersection) *intersection = ray.origin + ray.dir*minLength;
            return minLength;
        }
    }
    return -1; // Should never reach this point
}