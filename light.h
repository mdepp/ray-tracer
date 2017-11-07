#pragma once

#include "vec.h"

struct PointLight
{
    PointLight() = default;
    PointLight(vec3<> pos, float in, vec3<> col) :
        position(pos), intensity(in), colour(col) {}
    vec3<> position;
    float intensity;
    vec3<> colour;
};
struct DirectionalLight
{
    DirectionalLight() = default;
    DirectionalLight(vec3<> dir, vec3<> col) :
        direction(dir), colour(col) {}
    vec3<> direction;
    vec3<> colour;
};
struct AmbientLight
{
    AmbientLight() = default;
    AmbientLight(vec3<> col) :
        colour(col) {}
    vec3<> colour;
};
