#pragma once
#include "array.h"
#include "object.h"
#include "light.h"
#include "windowframework.h"
#include "util.h"
#include "vec.h"

template<uint16_t NumObjects, uint16_t NumLights>
class RayTracer
{
public:
    RayTracer();
    ~RayTracer();

    template<typename T, typename...Args>
    T* addObject(Args...args);
    template<typename...Args>
    PointLight* addLight(Args... args);
    bool render(WindowFramework* fw);
private:

    /*
    * Returns a pointer (from m_objects) of the object that first intersects
    * a ray. If there is no such intersection, nullptr is returned instead.
    */
    Object* getFirstIntersection(Ray ray);
    /*
    * Cast a ray and recursively trace its path. Returns the colour "seen"
    * with the ray.
    */
    vec3<> castRay(Ray ray, float intensity, uint16_t recursionDepth);

    /*
    * Get colour contributed by lights given a point and normal
    */
    vec3<> getLighting(vec3<> point, vec3<> normal);

    vec3<> m_backgroundColour;
    const float m_minIntensityThreshold;
    const uint16_t m_maxRecursionDepth;
    // If an object is closer than this to the origin of a ray, intersection is not registered (this stops rays colliding directly
    // after reflecting).
    const float m_minRayLength;

    const uint16_t m_maxObjects = NumObjects;
    const uint16_t m_maxLights = NumLights;
    Array<Object*, NumObjects> m_objects;
    Array<PointLight*, NumLights> m_lights;
};

template<uint16_t NumObjects, uint16_t NumLights>
vec3<> RayTracer<NumObjects, NumLights>::getLighting(vec3<> point, vec3<> normal)
{
    //return vec3<>(1.f, 1.f, 1.f);
    vec3<> total(0.f, 0.f, 0.f);
    for (auto& light : m_lights)
    {
        auto diff = light->position - point;
        total += light->colour * util::max(dot(normal, normalize(diff)), 0.f) * light->intensity / diff.length2();
    }
    return total;
}

template<uint16_t NumObjects, uint16_t NumLights>
RayTracer<NumObjects, NumLights>::RayTracer()
    : m_backgroundColour(0.f, 0.f, 0.f),
    m_minIntensityThreshold(0.001f),
    m_maxRecursionDepth(9),
    m_minRayLength(0.001)
{
}
template<uint16_t NumObjects, uint16_t NumLights>
RayTracer<NumObjects, NumLights>::~RayTracer()
{
    for (auto o : m_objects)
        delete o;
    for (auto l : m_lights)
        delete l;
}

template<uint16_t NumObjects, uint16_t NumLights>
bool RayTracer<NumObjects, NumLights>::render(WindowFramework * fw)
{
    const float viewportWidth = 10.f;
    const float viewportHeight = viewportWidth / float(fw->width())*fw->height();
    const float viewportDepth = 10.f;

    for (decltype(fw->width()) pixelX = 0; pixelX < fw->width(); ++pixelX)
    {
        for (decltype(fw->height()) pixelY = 0; pixelY < fw->height(); ++pixelY)
        {
            float x = ((int16_t)pixelX - (int16_t)fw->width() / 2)*viewportWidth / fw->width();
            float y = ((int16_t)pixelY - (int16_t)fw->height() / 2)*viewportHeight / fw->height();

            Ray ray({ 0, 0, 0 }, normalize(vec3<>(x, y, viewportDepth)));
            fw->drawPixel(pixelX, pixelY, castRay(ray, 1.f, 1));
            if (!fw->tick()) return false;
        }
    }
    return true;
}

template<uint16_t NumObjects, uint16_t NumLights>
Object * RayTracer<NumObjects, NumLights>::getFirstIntersection(Ray ray)
{
    Object* closestObject = nullptr;
    float closestDistance = -1.f;

    for (auto object : m_objects)
    {
        if (object) // Only consider used objects
        {
            auto distance = object->intersect(ray, nullptr);
            if (distance >= m_minRayLength) // Only consider objects that actually intersect ray
            {
                if (!closestObject || distance < closestDistance)
                {
                    closestObject = object;
                    closestDistance = distance;
                }
            }
        }
    }
    return closestObject;
}

template<uint16_t NumObjects, uint16_t NumLights>
vec3<> RayTracer<NumObjects, NumLights>::castRay(Ray ray, float intensity, uint16_t recursionDepth)
{
    if (intensity < m_minIntensityThreshold || recursionDepth > m_maxRecursionDepth) // Base case
        return m_backgroundColour;

    // Get the first object that intersects this ray
    auto intersectingObject = getFirstIntersection(ray);

    if (!intersectingObject)
        return m_backgroundColour; // Stop if no intersection took place

                                   // Get detailed information of the intersection
    IntersectionData id;
    if (intersectingObject->intersect(ray, &id) < 0)
        util::debugPrint("castRay() could not find an intersection for supposedly intersecting object.");
    // Calculate next ray and cast it
    auto reflectedRay = Ray(id.intersection, -reflectNormalized(ray.dir, id.normal));
    auto reflectedIntensity = intensity*id.reflectionCoefficient;

    auto lighting = getLighting(id.intersection, id.normal);

    return id.colour*lighting*(1.f - id.reflectionCoefficient)
        + castRay(reflectedRay, reflectedIntensity, recursionDepth + 1)*id.reflectionCoefficient;
}

template<uint16_t NumObjects, uint16_t NumLights>
template<typename T, typename ...Args>
T * RayTracer<NumObjects, NumLights>::addObject(Args ...args)
{
    if (m_objects.add(new T(util::forward<Args>(args)...)))
    {
        return static_cast<T*>(m_objects.back());
    }
    else
    {
        return nullptr;
    }
    /*for (int i = 0; i < m_maxObjects; ++i)
    {
        if (!m_objects[i])
        {
            m_objects[i] = new T(util::forward<Args>(args)...);
            return static_cast<T*>(m_objects[i]);
        }
    }
    return nullptr;*/
}

template<uint16_t NumObjects, uint16_t NumLights>
template<typename ...Args>
PointLight* RayTracer<NumObjects, NumLights>::addLight(Args ...args)
{
    if (m_lights.add(new PointLight(util::forward<Args>(args)...)))
    {
        return m_lights.back();
    }
    else
    {
        return nullptr;
    }
}