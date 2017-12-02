/*
 * File: sphere.h
 * Authors: Alexander Epp (1487716) and Mitchell Epp (1498821)
 * Project: CMPUT274 Final Project
 * Description: Implement the Object interface to define a sphere.
 */

#pragma once
#include "object.h"
#include "ray.h"
#include "vec.h"

class Sphere : public Object
{
public:
    Sphere(fvec3 center, float radius, fvec3 colour, float reflectionCoefficient);
    float intersect(Ray ray, IntersectionData* intersectionData) override;

private:
    fvec3 m_center;
    float m_radius;
    fvec3 m_colour;
    float m_reflectionCoefficient;
};