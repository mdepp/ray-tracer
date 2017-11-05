#pragma once
#include "object.h"
#include "ray.h"
#include "vec.h"

class Sphere : public Object
{
public:
    Sphere(vec3<> center, float radius, vec3<> colour, float reflectionCoefficient);
    float intersect(Ray ray, IntersectionData* intersectionData) override;

private:
    vec3<> m_center;
    float m_radius;
    vec3<> m_colour;
    float m_reflectionCoefficient;
};