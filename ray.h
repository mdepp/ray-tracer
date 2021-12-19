/*
 * File: ray.h
 * Authors: Alexander Epp, Rain Epp
 * Project: CMPUT274 Final Project
 * Description: Declaration of Ray struct
 */

#pragma once
#include "vec.h"
/*
 * Store a ray as an origin and direction (expected to be a unit vector)
 */
struct Ray
{
    Ray(fvec3 o, fvec3 d)
        : origin(o), dir(d)
    {}
    fvec3 origin;
    fvec3 dir;
};