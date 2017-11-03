#pragma once
#include "object.h"
#include "ray.h"
#include "util.h"

class Sphere : public Object
{
public:
    Sphere(vec3<> center, float radius);
    bool intersect(Ray ray, vec3<>& intersection) override;

private:
    vec3<> m_center;
    float m_radius;
};