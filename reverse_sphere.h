#pragma once
#include "object.h"
#include "ray.h"
#include "sphere.h"
#include "util.h"
#include "vec.h"

class ReverseSphere : public Object
{
public:
    template<typename...Args>
    ReverseSphere(Args&& ...args)
        : Object(),
          sphere(util::forward<Args>(args)...)
    {
    }

    float intersect(Ray ray, IntersectionData* intersectionData)
    {
        auto distance = sphere.intersect(ray, intersectionData);
        if (distance > 0 && intersectionData)
        {
            // Invert normals
            intersectionData->normal = -intersectionData->normal; 
        }
        return distance;
    }

private:
    Sphere sphere;
};