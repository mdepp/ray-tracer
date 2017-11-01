#pragma once
#include "object.h"
#include "ray.h"
#include <glm/glm.hpp>

class Sphere : public Object
{
public:
    Sphere(glm::vec3 center, float radius);
    bool intersect(Ray ray, glm::vec3& intersection) override;

private:
    glm::vec3 m_center;
    float m_radius;
};