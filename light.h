#pragma once

#include "vec.h"

struct PointLight
{
    PointLight() = default;
    PointLight(fvec3 pos, float in, fvec3 col) :
        position(pos), intensity(in), colour(col) {}
    fvec3 position;
    float intensity;
    fvec3 colour;
};
struct DirectionalLight
{
    DirectionalLight() = default;
    DirectionalLight(fvec3 dir, fvec3 col) :
        direction(dir), colour(col) {}
    fvec3 direction;
    fvec3 colour;
};
struct AmbientLight
{
    AmbientLight() = default;
    AmbientLight(fvec3 col) :
        colour(col) {}
    fvec3 colour;
};
