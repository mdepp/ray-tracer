#pragma once
#include "object.h"
#include "windowframework.h"
#include "util.h"
#include "vec.h"

template<uint16_t NumObjects>
class RayTracer
{
public:
    RayTracer();
    ~RayTracer();

    template<typename T, typename...Args>
    T* addObject(Args...args);
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

    vec3<> m_backgroundColour;
    const float m_minIntensityThreshold;
    const uint16_t m_maxRecursionDepth;
    // If an object is closer than this to the origin of a ray, intersection is not registered (this stops rays colliding directly
    // after reflecting.
    const float m_minRayLength;

    static const uint16_t m_maxObjects = NumObjects;
    Object* m_objects[m_maxObjects];
};

template<uint16_t NumObjects>
RayTracer<NumObjects>::RayTracer()
    : m_backgroundColour(0.f, 0.f, 0.f),
    m_minIntensityThreshold(0.001f),
    m_maxRecursionDepth(9),
    m_minRayLength(0.001)
{
    for (int i = 0; i < m_maxObjects; ++i)
        m_objects[i] = nullptr;
}
template<uint16_t NumObjects>
RayTracer<NumObjects>::~RayTracer()
{
    for (int i = 0; i < m_maxObjects; ++i)
        delete m_objects[i];
}

template<uint16_t NumObjects>
bool RayTracer<NumObjects>::render(WindowFramework * fw)
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

template<uint16_t NumObjects>
Object * RayTracer<NumObjects>::getFirstIntersection(Ray ray)
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

template<uint16_t NumObjects>
vec3<> RayTracer<NumObjects>::castRay(Ray ray, float intensity, uint16_t recursionDepth)
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

    return id.colour*(1.f - id.reflectionCoefficient)
        + castRay(reflectedRay, reflectedIntensity, recursionDepth + 1)*id.reflectionCoefficient;
}

template<uint16_t NumObjects>
template<typename T, typename ...Args>
T * RayTracer<NumObjects>::addObject(Args ...args)
{
    for (int i = 0; i < m_maxObjects; ++i)
    {
        if (!m_objects[i])
        {
            m_objects[i] = new T(util::forward<Args>(args)...);
            return static_cast<T*>(m_objects[i]);
        }
    }
    return nullptr;
}