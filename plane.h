#pragma once
#include "object.h"
#include "ray.h"
#include "vec.h"

class Plane : public Object
{
public:
	  /*
		 * Init plane. 'normal' does not have to be normalized.
		 */
    Plane(vec3<> origin, vec3<> normal, vec3<> colour, float reflectionCoefficient);
    float intersect(Ray ray, IntersectionData* intersectionData) override;

private:
    vec3<> m_origin;
    vec3<> m_normal;
    vec3<> m_colour;
    float m_reflectionCoefficient;
};
