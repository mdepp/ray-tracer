/*
 * File: raytracer.h
 * Authors: Alexander Epp (1487716) and Mitchell Epp (1498821)
 * Project: CMPUT274 Final Project
 * Description: Contains the RayTracer class, which traces the path of rays cast
 *              by the Camera, and renders the results to a WindowFramework.
 */

#pragma once
#include "array.h"
#include "object.h"
#include "light.h"
#include "windowframework.h"
#include "util.h"
#include "vec.h"
#include "camera.h"

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
class RayTracer
{
public:
    RayTracer(int);
    ~RayTracer();

    /*
     * Add an object to the scene to be rendered (the object is added as
     * a pointer instead of using variadic templates like the other methods
     * since this allows allocating memory for each object statically).
     * If the object pointer was successfully added, it is returned; otherwise
     * nullptr is returned.
     * RayTracer is not responsible for deleting these objects.
     */
    template<typename T>
    T* addObject(T* object);
    /*
     * Add a point light to the scene to be rendered. Arguments are the
     * arguments to PointLight's c-tor.
     */
    template<typename...Args>
    bool addPointLight(Args&& ...args);
    /*
     * Add a directional light to the scene to be rendered. Arguments are the
     * arguments to DirectionalLight's c-tor. Returns true iff the light was
     * successfully added.
     */
    template<typename...Args>
    bool addDirectionalLight(Args&& ...args);
    /*
     * Set the ambient light of the scene to be rendered. Arguments are the 
     * arguments to AmbientLight's c-tor. Returns true iff the light was
     * successfully added.
     */
    template<typename...Args>
    void setAmbientLight(Args&& ...args);
    /*
     * Render the scene to a given WindowFramework, with rays cast by Camera.
     * Returns false if WindowFramework indicates the application should quit,
     * and true otherwise.
     */
    bool render(WindowFramework* fw, Camera* cam);

    void setRecursionDepth(int);
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
    const float m_minIntensityThreshold; // Stop recursing once light intensity is below this threshold
    const uint16_t m_maxRecursionDepth;
    const float m_minRayLength; // Rays must intersect at a point beyond this length

    Array<Object*, NumObjects> m_objects;
    Array<PointLight, NumPointLights> m_pointLights;
    Array<DirectionalLight, NumDirectionalLights> m_directionalLights;
    AmbientLight m_ambientLight;
};

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::RayTracer(int maxRecursionDepth)
    : m_backgroundColour(0.f, 0.f, 0.f),
      m_minIntensityThreshold(0.001f),
      m_maxRecursionDepth(maxRecursionDepth),
      m_minRayLength(0.001)
{
}
template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::~RayTracer()
{
    // RayTracer is not responsible for deleting objects (and lights are cleaned
    // up automatically).
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
fvec3 RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::getLighting(fvec3 point, fvec3 normal)
{
    fvec3 total(0.f, 0.f, 0.f);
    for (auto& light : m_pointLights)
    {
        auto diff = light.position - point;
        auto dir = normalize(diff);
        // A shadow ray test would go here
        //normal += fvec3((rand()%2-1)/100.f, (rand()%2-1)/100.f, (rand()%2-1)/100.f);

        // Only accumulate light if light ray is on the 'outside' of the object
        // (toward the normal). Since a 2D object (such as a plane) can be viewed
        // from both sides, *both* sides will be lit, but only by lights from
        // *one* side.
        total += light.colour * util::max(dot(normal, dir), 0.f) * util::min(light.intensity / diff.length2(), 1.f);
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
    if (dot(ray.dir, normal) < 0)
        return { ray.origin, -reflectNormalized(ray.dir, normal) };
    else
        return { ray.origin, -reflectNormalized(ray.dir, -normal) };
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
bool RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::render(WindowFramework* fw, Camera* cam)
{
    for (decltype(cam->nPixelsX()) pixelX = 0; pixelX < cam->nPixelsX(); ++pixelX)
    {
        for (decltype(cam->nPixelsY()) pixelY = 0; pixelY < cam->nPixelsY(); ++pixelY)
        {
            Ray ray = cam->getPixelRay(pixelX, pixelY);
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
            
            // Objects must be a certain distance from ray origin (otherwise
            // rays would reflect several times in a row from the same
            // intersection point).
            if (distance >= m_minRayLength)
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

    // Stop if no intersection took place
    if (!intersectingObject)
        return m_ambientLight.colour;

    // Get detailed information of the intersection
    IntersectionData id;
    if (intersectingObject->intersect(ray, &id) < 0)
        util::debugPrint("castRay() could not find an intersection for supposedly intersecting object.");

    // Calculate reflected ray
    auto reflectedRay = reflectRay({ id.intersection, ray.dir }, id.normal);
    auto reflectedIntensity = intensity*id.reflectionCoefficient;
    
    // Calculate diffuse lighting at reflection point
    auto lighting = getLighting(id.intersection, id.normal);

    // Cast the reflected ray
    return lighting*id.colour*(1.f - id.reflectionCoefficient)
        + castRay(reflectedRay, reflectedIntensity, recursionDepth+1)*id.reflectionCoefficient;
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
template<typename T>
T * RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::addObject(T* object)
{
    if (m_objects.add(object))
    {
        return object;
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

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
void RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::setRecursionDepth(int rd)
{
    m_maxRecursionDepth = rd;
}
