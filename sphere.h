#pragma once
#include "object.h"
#include "ray.h"
#include "vec.h"

class Sphere : public Object
{
public:
    Sphere(vec3<> center, float radius);
    float intersect(Ray ray, vec3<>* intersection) override;

private:
    vec3<> m_center;
    float m_radius;
};