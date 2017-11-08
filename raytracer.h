#pragma once
#include "array.h"
#include "object.h"
#include "light.h"
#include "windowframework.h"
#include "util.h"
#include "vec.h"

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
class RayTracer
{
public:
    RayTracer();
    ~RayTracer();

    template<typename T, typename...Args>
    T* addObject(Args&& ...args);

    template<typename...Args>
    bool addPointLight(Args&& ...args);

    template<typename...Args>
    bool addDirectionalLight(Args&& ...args);

    template<typename...Args>
    void setAmbientLight(Args&& ...args);

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
    fvec3 castRay(Ray ray, float intensity, uint16_t recursionDepth);

    /*
    * Get colour contributed by lights given a point and normal
    */
    fvec3 getLighting(fvec3 point, fvec3 normal);

    /*
     * Reflect a ray from a normal (double-sided)
     */
    Ray reflectRay(Ray ray, fvec3 normal);

    fvec3 m_backgroundColour;
    const float m_minIntensityThreshold;
    const uint16_t m_maxRecursionDepth;
    // If an object is closer than this to the origin of a ray, intersection is not registered (this stops rays colliding directly
    // after reflecting).
    const float m_minRayLength;

    Array<Object*, NumObjects> m_objects;
    Array<PointLight, NumPointLights> m_pointLights;
    Array<DirectionalLight, NumDirectionalLights> m_directionalLights;
    AmbientLight m_ambientLight;
};

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::RayTracer()
    : m_backgroundColour(0.f, 0.f, 0.f),
    m_minIntensityThreshold(0.001f),
    m_maxRecursionDepth(40),
    m_minRayLength(0.001)
{
}
template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::~RayTracer()
{
    for (auto o : m_objects)
        delete o;
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
fvec3 RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::getLighting(fvec3 point, fvec3 normal)
{
    //return fvec3(1.f, 1.f, 1.f);
    fvec3 total(0.f, 0.f, 0.f);
    for (auto& light : m_pointLights)
    {
        auto diff = light.position - point;
        total += light.colour * util::max(dot(normal, normalize(diff)), 0.f) * util::min(light.intensity / diff.length2(), 1.f);
    }
    for (auto& light : m_directionalLights)
    {
        total += light.colour * util::max(dot(normal, light.direction), 0.f);
    }
    total += m_ambientLight.colour;

    return total;
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
Ray RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::reflectRay(Ray ray, fvec3 normal)
{
    //if (dot(ray.dir, normal) < 0)
        return { ray.origin, -reflectNormalized(ray.dir, normal) };
    //else
    //    return { ray.origin, -reflectNormalized(ray.dir, -normal) };
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
bool RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::render(WindowFramework * fw)
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

            Ray ray({ 0, 0, 0 }, normalize(fvec3(x, y, viewportDepth)));
            fw->drawPixel(pixelX, pixelY, castRay(ray, 1.f, 1));
            if (!fw->tick()) return false;
        }
    }
    return true;
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
Object * RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::getFirstIntersection(Ray ray)
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

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
fvec3 RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::castRay(Ray ray, float intensity, uint16_t recursionDepth)
{
    if (intensity < m_minIntensityThreshold || recursionDepth > m_maxRecursionDepth) // Base case
        return m_ambientLight.colour;

    // Get the first object that intersects this ray
    auto intersectingObject = getFirstIntersection(ray);

    if (!intersectingObject)
        return m_ambientLight.colour; // Stop if no intersection took place

                                   // Get detailed information of the intersection
    IntersectionData id;
    if (intersectingObject->intersect(ray, &id) < 0)
        util::debugPrint("castRay() could not find an intersection for supposedly intersecting object.");
    // Calculate next ray and cast it
    auto reflectedRay = reflectRay({ id.intersection, ray.dir }, id.normal); // Ray(id.intersection, -reflectNormalized(ray.dir, id.normal));
    auto reflectedIntensity = intensity*id.reflectionCoefficient;

    auto lighting = getLighting(id.intersection, id.normal);

    return lighting * id.colour*(1.f - id.reflectionCoefficient)
        + castRay(reflectedRay, reflectedIntensity, recursionDepth + 1)*id.reflectionCoefficient;
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
template<typename T, typename ...Args>
T * RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::addObject(Args&& ...args)
{
    if (m_objects.add(new T(util::forward<Args>(args)...)))
    {
        return static_cast<T*>(m_objects.back());
    }
    else
    {
        return nullptr;
    }
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
template<typename ...Args>
bool RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::addPointLight(Args&& ...args)
{
    return m_pointLights.add(util::forward<Args>(args)...);
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
template<typename ...Args>
bool RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::addDirectionalLight(Args&& ...args)
{
    return m_directionalLights.add(util::forward<Args>(args)...);
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
template<typename ...Args>
void RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::setAmbientLight(Args&& ...args)
{
    m_ambientLight = AmbientLight(util::forward<Args>(args)...);
}
