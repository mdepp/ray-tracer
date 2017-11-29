/*
 * Note: This class was based in part on code used in our project 2 part 1 submission
 */

#pragma once

#include "util.h"

template<typename T>
class vec3;

template <typename T=float>
class vec4
{
public:
    T x, y, z, w;

    vec4() : x(0), y(0), z(0), w(0) {}
    vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
    explicit vec4(const vec3<T>& v, T _w) : x(v.x), y(v.y), z(v.z), w(_w) {}
    explicit vec4(const vec3<T>& v) : vec4(v, 0) {}

    T length2() const noexcept
    {
        return x*x + y*y + z*z + w*w;
    }
    T length() const noexcept
    {
        return sqrt(length2());
    }

    bool operator == (const vec4<T>& other) const
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}
	bool operator != (const vec4<T>& other) const
	{
        return x != other.x || y != other.y || z != other.z || w != other.w;
	}
    vec4<T> operator / (T n) const
    {
        return vec4<T>(x/n, y/n, z/n, w/n);
    }
    vec4<T> operator * (T n) const
    {
        return vec4<T>(x*n, y*n, z*n, w*n);
    }
	vec4<T> operator - (const vec4<T>& other) const
	{
        return vec2<T>(x - other.x, y - other.y, z - other.z, w - other.w);
	}
	vec4<T> operator + (const vec4<T>& other) const
	{
        return vec4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
	}
    vec4<T> operator-() const noexcept
    {
        return vec4<T>(-x, -y, -z, -w);
    }
    vec4<T> operator*(const vec4<T>& other) const noexcept
    {
        return vec4<T>(x*other.x, y*other.y, z*other.z, w*other.w);
    }
};

template <typename T=float>
class vec3
{
public:
    union
    {
        T x, r;
    };
    union
    {
        T y, g;
    };
    union
    {
        T z, b;
    };

    vec3() : x(0), y(0), z(0) {}
    vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    explicit vec3(const vec4<T>& v) : x(v.x), y(v.y), z(v.z) {}

    T length2() const noexcept
    {
        return x*x + y*y + z*z;
    }
    T length() const noexcept
    {
        return sqrt(length2());
    }

    bool operator == (const vec3<T>& other) const noexcept
	{
		return x == other.x && y == other.y && z == other.z;
	}
	bool operator != (const vec3<T>& other) const noexcept
	{
		return x != other.x || y != other.y || z != other.z;
	}
	vec3<T> operator / (T n) const noexcept
	{
		return vec3<T>(x / n, y / n, z / n);
	}
    vec3<T> operator * (T n) const noexcept
    {
        return vec3<T>(x * n, y * n, z * n);
    }
	vec3<T> operator - (const vec3<T>& other) const noexcept
	{
		return vec3<T>(x - other.x, y - other.y, z - other.z);
	}
	vec3<T> operator + (const vec3<T>& other) const noexcept
	{
		return vec3<T>(x + other.x, y + other.y, z + other.z);
	}
    vec3<T> operator-() const noexcept
    {
        return vec3<T>(-x, -y, -z);
    }
    vec3<T> operator * (const vec3<T>& other) const noexcept
    {
        return vec3<T>(x*other.x, y*other.y, z*other.z);
    }
    vec3<T>& operator += (const vec3<T>& other) noexcept
    {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }
};

template<typename T>
T dot(const vec4<T>& v1, const vec4<T>& v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

template<typename T>
T dot(const vec3<T>& v1, const vec3<T>& v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
template<typename T>
vec3<T> cross(const vec3<T>& u, const vec3<T>& v)
{
    return
    {
        u.y*v.z - u.z*v.y,
        u.z*v.x - u.x*v.z,
        u.x*v.y - u.y*v.x
    };
}


template<typename T>
T distance2(const vec4<T>& v1, const vec4<T>& v2)
{
    return (v1 - v2).length2();
}

template<typename T>
T distance2(const vec3<T>& v1, const vec3<T>& v2)
{
    return (v1 - v2).length2();
}

template<typename T>
vec4<T> normalize(const vec4<T>& v)
{
    return v / v.length();
}
template<typename T>
vec3<T> normalize(const vec3<T>& v)
{
    return v / v.length();
}

/*
 * Reflect about a normalized axis vector
 */
template<typename T>
vec4<T> reflectNormalized(const vec4<T>& v, const vec4<T>& axis)
{
    return axis*dot(v, axis)*2 - v;
}
template<typename T>
vec3<T> reflectNormalized(const vec3<T>& v, const vec3<T>& axis)
{
    return axis*dot(v, axis)*2 - v;
}

using fvec3 = vec3<float>;
using fvec4 = vec4<float>;