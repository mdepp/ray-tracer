#include "plane.h"
#include "ray.h"
#include "util.h"
#include "vec.h"

Plane::Plane(fvec3 origin, fvec3 normal, fvec3 colour, bool transparent, float refractiveIndex)
    : Object(),
    m_origin(origin),
    m_normal(normalize(normal)),
    m_colour(colour),
    m_transparent(transparent), m_refractiveIndex(refractiveIndex)
{

}
float Plane::intersect(Ray ray, IntersectionData* intersectionData)
{
    // https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    const float epsilon = 0.003;
    auto dotProduct = dot(m_normal, ray.dir);
    if (util::abs(dotProduct) == 0) // Ray is parallel to plane
    {
        // No intersection if parallel
        return -1;
    }

    auto distance = dot(m_origin - ray.origin, m_normal) / dotProduct;
    if (distance > 0 && intersectionData)
    {
      intersectionData->intersection = ray.origin + ray.dir*distance;
      auto r = (int(intersectionData->intersection.x) % 2 + 2) % 2;
      auto g = (int(intersectionData->intersection.y) % 2 + 2) % 2;
      auto b = (int(intersectionData->intersection.z) % 2 + 2) % 2;

      intersectionData->colour = fvec3(r, g, b);// m_colour;
      intersectionData->normal = m_normal;
      intersectionData->transparent = m_transparent;
      intersectionData->refractiveIndex = m_refractiveIndex;
    }
    return distance;
}
