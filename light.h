#pragma once

#include "vec.h"

struct PointLight
{
    PointLight(vec3<> pos, float in, vec3<> col) : 
        position(pos), intensity(in), colour(col) {}
    vec3<> position;
    float intensity;
    vec3<> colour;
};