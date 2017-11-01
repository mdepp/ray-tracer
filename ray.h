#pragma once
#include <glm/glm.hpp>

/*
 * Store a ray as an origin and direction (expected to be a unit vector)
 */
struct Ray
{
    glm::vec3 origin;
    glm::vec3 dir;
};