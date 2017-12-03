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

struct RefractionData;

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
class RayTracer
{
public:
    /*
     * Initialize ray-tracer by specifying maximum recursion depth
     */
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
    
    /*
     * Set the maximum recursion depth of the ray-tracing algorithm.
     */
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

    /*
     * Refract a ray from a normal (double-sided)
     */
    RefractionData refractRay(Ray ray, fvec3 normal, float refractiveIndex);

    fvec3 m_backgroundColour;
    const float m_minIntensityThreshold; // Stop recursing once light intensity is below this threshold
    uint16_t m_maxRecursionDepth;
    const float m_minRayLength; // Rays must intersect at a point beyond this length

    Array<Object*, NumObjects> m_objects;
    Array<PointLight, NumPointLights> m_pointLights;
    Array<DirectionalLight, NumDirectionalLights> m_directionalLights;
    AmbientLight m_ambientLight;
};

struct RefractionData
{
    RefractionData(Ray r, bool t)
        : ray(r), tir(t) {}
    Ray ray;
    bool tir; // total internal reflection
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
    auto isShadowed = [this](const PointLight& light, fvec3 pos) -> bool
    {
        for (auto object : m_objects)
        {
            auto toLight = light.position - pos;
            auto distance = object->intersect({ pos, normalize(toLight) }, nullptr, m_minRayLength);
            if (distance > 0 && distance < (toLight.length() - m_minRayLength)) // Collision is between light and object
            {
                return true;
            }
        }
        return false;
    };

    fvec3 total(0.f, 0.f, 0.f);
    for (auto& light : m_pointLights)
    {
        auto diff = light.position - point;
        auto dir = normalize(diff);
        // A shadow ray test would go here
        //normal += fvec3((rand()%2-1)/100.f, (rand()%2-1)/100.f, (rand()%2-1)/100.f);

        // Only accumulate light if light ray is on the 'outside' of the object
        // (direction in which the surface normal points). Since a 2D object
        // (such as a plane) can be viewed from both sides, *both* sides will be
        // lit, but only by lights from *one* side.
        if (!isShadowed(light, point))
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
RefractionData RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::refractRay(Ray ray, fvec3 normal, float refractiveIndex)
{
    // This algorithm is as described in "An Improved Illumination Model for
    // Shaded Display" (published 1980) by Turner Whitted, last accessed
    // December 2017 at https://pdfs.semanticscholar.org/78e7/620a01ecaecc62b00bf47f86c6822e3d9625.pdf

    auto v_dot_n = dot(ray.dir, normal);
    if (v_dot_n == 0) // Ray is parallel to surface, so there is no refraction
    {
        return { ray, false };
    }
    else if (v_dot_n > 0) // Ray is leaving object, so flip normal and index of refraction
    {
        v_dot_n = -v_dot_n;
        normal = -normal;
        refractiveIndex = 1.f / refractiveIndex;
    }

    auto v_prime = -ray.dir / v_dot_n; // V' = V/|V.N|
    auto radicand = util::pow2(refractiveIndex)*v_prime.length2() - (v_prime + normal).length2(); // k_n^2 |V'|^2 - |V'+N|^2
    if (radicand <= 0) // Total internal reflection
    {
        return { ray, true };
    }
    auto k_f = 1.f / sqrt(radicand); // (k_n^2 |V'|^2 - |V'+N|^2)^(-1/2)
    auto refractedRay = Ray(ray.origin, normalize((normal + v_prime)*k_f - normal)); // k_f (N+V') - N
    return { refractedRay, false };
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
            // Get first intersection with object beyond minRayLength
            auto distance = object->intersect(ray, nullptr, m_minRayLength);

            if (distance > 0) // An intersection was found
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
    // Stop if light intensity has decayed to insignificance, or after a fixed number of recursions.
    if (intensity < m_minIntensityThreshold || recursionDepth > m_maxRecursionDepth)
        return m_ambientLight.colour;

    // Get the first object that intersects this ray
    auto intersectingObject = getFirstIntersection(ray);

    // Stop if no intersection took place
    if (!intersectingObject)
        return m_ambientLight.colour;

    // Get detailed information of the intersection
    IntersectionData id;
    if (intersectingObject->intersect(ray, &id, m_minRayLength) < 0)
        util::debugPrint("castRay() could not find an intersection for supposedly intersecting object.");

    // Calculate reflected ray
    auto reflectedRay = reflectRay({ id.intersection, ray.dir }, id.normal);
    auto reflectionCoefficient = id.reflectionCoefficient;

    // Calculate refracted ray
    auto refractionData = refractRay({ id.intersection, ray.dir }, id.normal, 1.02f);
    auto refractedRay = refractionData.ray;
    auto transmissionCoefficient = id.transmissionCoefficient;
    if (refractionData.tir) // Total internal reflection
    {
        reflectionCoefficient += transmissionCoefficient;
        transmissionCoefficient = 0;
    }

    // Calculate diffuse lighting
    auto diffuseCoefficient = 1.f - reflectionCoefficient - transmissionCoefficient;
    auto diffuseLighting = getLighting(id.intersection, id.normal);

    // Cast the reflected and refracted rays
    return diffuseLighting*id.colour*diffuseCoefficient
        + castRay(reflectedRay, intensity*reflectionCoefficient, recursionDepth + 1)*reflectionCoefficient
        + castRay(refractedRay, intensity*transmissionCoefficient, recursionDepth + 1)*transmissionCoefficient;
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
