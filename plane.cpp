#include "plane.h"
#include "ray.h"
#include "util.h"
#include "vec.h"

Plane::Plane(vec3<> origin, vec3<> normal, vec3<> colour, float reflectionCoefficient)
  : Object(),
    m_origin(origin),
    m_normal(normalize(normal)),
    m_colour(colour),
    m_reflectionCoefficient(reflectionCoefficient)
{

}
float Plane::intersect(Ray ray, IntersectionData* intersectionData)
{
    // https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    const float epsilon = 0.003;
    auto dotProduct = dot(m_normal, ray.dir);
    if (util::abs(dotProduct) < epsilon) // Ray is parallel to plane
    {
      if (intersectionData)
      {
        intersectionData->intersection = ray.origin; // can intersect anywhere
        intersectionData->colour = m_colour;
        intersectionData->normal = m_normal;
        intersectionData->reflectionCoefficient = m_reflectionCoefficient;
      }
      return 0;
    }

    auto distance = dot(m_origin - ray.origin, m_normal) / dotProduct;
    if (distance > 0 && intersectionData)
    {
      intersectionData->intersection = ray.origin + ray.dir*distance;
      intersectionData->colour = m_colour;
      intersectionData->normal = m_normal;
      intersectionData->reflectionCoefficient = m_reflectionCoefficient;
    }
    return distance;
}
