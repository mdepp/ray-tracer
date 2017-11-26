#pragma once
#include "array.h"
#include "object.h"
#include "light.h"
#include "windowframework.h"
#include "util.h"
#include "vec.h"


#include <cassert>

// Since can't return tuples properly without C++17 and the standard library
struct RefractionData
{
    RefractionData(Ray r, bool t)
        : ray(r), tir(t) {}
    Ray ray;
    bool tir; // total internal reflection
};

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
class RayTracer
{
public:
    RayTracer();
    ~RayTracer();

    template<typename T/*, typename...Args*/>
    T* addObject(T* object/*Args&& ...args*/);

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
    * Returns true if the given line segment intersects with an object
    */
    bool segmentIntersects(Ray ray, float length);
    /*
    * Cast a ray and recursively trace its path. Returns the colour "seen"
    * with the ray.
    */
    fvec3 castRay(Ray ray, float intensity, uint16_t recursionDepth, bool isRefractive=false);

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
    m_maxRecursionDepth(90),
    m_minRayLength(0.001)
{
}
template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::~RayTracer()
{
    /*for (auto o : m_objects)
        delete o;*/
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
fvec3 RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::getLighting(fvec3 point, fvec3 normal)
{
    //return fvec3(1.f, 1.f, 1.f);
    fvec3 total(0.f, 0.f, 0.f);
    for (auto& light : m_pointLights)
    {
        auto diff = light.position - point;
        auto dir = normalize(diff);
        if (!segmentIntersects(Ray(light.position, dir), diff.length()))
        {   
            //normal += fvec3((rand()%2-1)/100.f, (rand()%2-1)/100.f, (rand()%2-1)/100.f);
            total += light.colour * util::max(dot(normal, dir), 0.f) * util::min(light.intensity / diff.length2(), 1.f);
        }
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

template <uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
bool RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::segmentIntersects(Ray ray, float length)
{
    return false;
    for (auto object : m_objects)
    {
        if (object)
        {
            auto distance = object->intersect(ray, nullptr);
            if (distance >= m_minRayLength && distance <= length)
            {
                return true;
            }
        }
    }
    return false;
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
fvec3 RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::castRay(Ray ray, float intensity, uint16_t recursionDepth, bool isRefractive)
{
    if (intensity < m_minIntensityThreshold || recursionDepth > m_maxRecursionDepth) // Base case
        return { 0.f, 1.f, 0.f };// m_ambientLight.colour;

    // Get the first object that intersects this ray
    auto intersectingObject = getFirstIntersection(ray);

    if (!intersectingObject)
        return m_ambientLight.colour; // Stop if no intersection took place

    // Get detailed information of the intersection
    IntersectionData id;
    if (intersectingObject->intersect(ray, &id) < 0)
        util::debugPrint("castRay() could not find an intersection for supposedly intersecting object.");
    // HACK -- manually sets refractive index (change this)
    id.refractiveIndex = 1.1f;
    
    // Calculate next ray and cast it
    auto reflectedRay = reflectRay({ id.intersection, ray.dir }, id.normal); // Ray(id.intersection, -reflectNormalized(ray.dir, id.normal));
    auto reflectionCoefficient = id.reflectionCoefficient;

    auto refractRay = [this](Ray ray, fvec3 normal, float refractiveIndex) -> RefractionData
    {/*
        auto v_dot_n = dot(ray.dir, normal);
        if (v_dot_n == 0) // Parallel to surface, so no refraction
        {
            return { ray, false };
        }
        else if (v_dot_n > 0) // Take into account ray leaving object
        {
            v_dot_n = -v_dot_n;
            normal = -normal;
            refractiveIndex = 1.f / refractiveIndex;
        }

        auto v_prime = -ray.dir/v_dot_n; // V' = V/|V.N|
        auto radicand = util::pow2(refractiveIndex)*v_prime.length2() - (v_prime + normal).length2(); // k_n^2 |V'|^2 - |V'+N|^2
        radicand = util::abs(radicand);
        if (radicand <= 0) // Total internal reflection
        {
            return { reflectRay(ray, normal), true };
        }
        auto k_f = 1.f / sqrt(radicand); // (k_n^2 |V'|^2 - |V'+N|^2)^(-1/2)

        auto refractedRay = Ray(ray.origin, normalize((normal+v_prime)*k_f - normal)); // k_f (N+V') - N
        return { refractedRay, false };*/
        /*
        // Assume object has air boundary
        auto c = -dot(normal, ray.dir);
        auto r = (c > 0) ? 1.f / refractiveIndex : refractiveIndex; // Going from air->object or object->air
                                                                          //auto r = 1.f / id.refractiveIndex;
        if (c < 0)
        {
            return { ray, false };
            c = -c; // c should be positive
            normal = -normal;
        }

        auto radicand = 1.f - util::pow2(r)*(1.f - util::pow2(c));
        if (radicand < 0) return { ray, true }; // total internal reflection

        return { { ray.origin, ray.dir*r + normal*(r*c - sqrt(radicand)) }, false };*/

        assert(util::abs(ray.dir.length2()-1.f) < 0.01);
        assert(util::abs(normal.length2()-1.f) < 0.01);

        if (dot(normal, ray.dir) > 0)
        {
            normal = -normal;
            refractiveIndex = 1.f / refractiveIndex;
        }

        auto term1 = (cross(normal, cross(-normal, ray.dir)))*(1.f / refractiveIndex);
        auto radicand = 1.f - util::pow2(1.f / refractiveIndex)*dot(cross(normal, ray.dir), cross(normal, ray.dir));
        if (radicand > 0)
        {
            auto term2 = -normal*sqrt(radicand);
            return { {ray.origin, normalize(term1 + term2)}, false };
        }
        else //tir
        {
            return { ray, false };
        }
    };



    auto refractionData = refractRay({ id.intersection, ray.dir }, id.normal, id.refractiveIndex);
    
    // =========================================================
    if (isRefractive)
    {
        refractionData.ray = { id.intersection, ray.dir };
        return castRay(refractionData.ray, intensity, recursionDepth + 1, false);
    }
    // ========================================================
    
    auto transmissionCoefficient = id.transmissionCoefficient;
    //refractionData.tir = true;
    if (refractionData.tir) // total internal reflection
    {
        reflectionCoefficient += transmissionCoefficient;
        transmissionCoefficient = 0.f;
    }

    auto diffuseCoefficient = (1.f - reflectionCoefficient - transmissionCoefficient);
    if (diffuseCoefficient < 0)
    {
        util::debugPrint("In castRay(), diffuse coefficient of light is negative: ", diffuseCoefficient);
    }

    //util::debugPrint(id.normal.x, ", ", id.normal.y, ", ", id.normal.z);
    auto lighting = getLighting(id.intersection, id.normal);
    
    refractionData.ray = { id.intersection, ray.dir };
    if (transmissionCoefficient > 0 && !refractionData.tir)
        return castRay(refractionData.ray, intensity, recursionDepth + 1, false);
    else if (transmissionCoefficient > 0 && refractionData.tir)
        return castRay(reflectedRay, intensity, recursionDepth + 1);
    else
        return castRay(reflectedRay, reflectionCoefficient*intensity, recursionDepth + 1)*reflectionCoefficient + lighting*id.colour*diffuseCoefficient;
}

template<uint16_t NumObjects, uint16_t NumPointLights, uint16_t NumDirectionalLights>
template<typename T/*, typename ...Args*/>
T * RayTracer<NumObjects, NumPointLights, NumDirectionalLights>::addObject(T* object/*Args&& ...args*/)
{
    if (m_objects.add(object/*new T(util::forward<Args>(args)...))*/))
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
