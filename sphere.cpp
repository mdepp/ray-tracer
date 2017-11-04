#include "sphere.h"
#include "ray.h"
#include "util.h"
#include "vec.h"

Sphere::Sphere(vec3<> center, float radius)
 : Object(), m_center(center), m_radius(radius)
{
}

bool Sphere::intersect(Ray ray, vec3<>& intersection)
{
    // As described in https://en.wikipedia.org/wiki/Line–sphere_intersection
    auto discriminant = pow2(dot(ray.dir, ray.origin-m_center))
        - distance2(ray.origin, m_center) + pow2(m_radius);

    if (discriminant < 0) return false;

    auto term1 = -dot(ray.dir, ray.origin-m_center);
    if (discriminant == 0) // Intersects once with sphere
    {
        if (term1 >= 0)
        {
            intersection = ray.origin + ray.dir*term1;
            return true;
        }
        else
        {
            return false;
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
                return false;
            }
            else
            {
                intersection = ray.origin + ray.dir*maxLength;
                return true;
            }
        }
        else
        {
            intersection = ray.origin + ray.dir*minLength;
            return true;
        }
    }
    return false; // Should never reach this point
}