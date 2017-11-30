/*
 * File: plane.h
 * Authors: Alexander Epp (1487716) and Mitchell Epp (1498821)
 * Project: CMPUT274 Final Project
 * Description: Implements the Object interface to define an infinite plane
 */

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
