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
    Plane(fvec3 origin, fvec3 normal, fvec3 colour, float reflectionCoefficient);
    float intersect(Ray ray, IntersectionData* intersectionData) override;

private:
    fvec3 m_origin;
    fvec3 m_normal;
    fvec3 m_colour;
    float m_reflectionCoefficient;
};