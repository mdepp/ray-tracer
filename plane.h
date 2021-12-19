/*
 * File: plane.h
 * Authors: Alexander Epp, Rain Epp
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
    Plane(fvec3 origin, fvec3 normal, fvec3 colour, float reflectionCoefficient, float transmissionCoefficient=0.f, float refractiveIndex=1.f);
    float intersect(Ray ray, IntersectionData* intersectionData, float epsilon) override;

private:
    fvec3 m_origin;
    fvec3 m_normal;
    fvec3 m_colour;
    float m_reflectionCoefficient, m_transmissionCoefficient;
    float m_refractiveIndex;
};
