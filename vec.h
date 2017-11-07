/*
 * Note: This class was based in part on code used in our project 2 part 1 submission
 */

#pragma once

#include "util.h"

template <typename T=float>
class vec2
{
public:
    union
    {
        T x, w;
    };
    union
    {
        T y, h;
    };

    vec2() : x(0), y(0) {}
    vec2(T _x, T _y) : x(_x), y(_y) {}

    T length2() const noexcept
    {
        return x*x + y*y;
    }
    T length() const noexcept
    {
        return sqrt(length2());
    }

    bool operator == (const vec2<T>& other) const
	{
		return x == other.x && y == other.y;
	}
	bool operator != (const vec2<T>& other) const
	{
		return x != other.x || y != other.y;
	}
    vec2<T> operator / (T n) const
    {
        return vec2<T>(x / n, y / n);
    }
    vec2<T> operator * (T n) const
    {
        return vec2<T>(x * n, y * n);
    }
	vec2<T> operator - (const vec2<T>& other) const
	{
		return vec2<T>(x - other.x, y - other.y);
	}
	vec2<T> operator + (const vec2<T>& other) const
	{
		return vec2<T>(x + other.x, y + other.y);
	}
    vec2<T> operator-() const noexcept
    {
        return vec2<T>(-x, -y);
    }
    vec2<T> operator * (const vec2<T>& other) const noexcept
    {
        return vec2<T>(x*other.x, y*other.y);
    }
};
typedef vec2<int> ivec2;
typedef vec2<float> fvec2;

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
T dot(const vec2<T>& v1, const vec2<T>& v2)
{
    return v1.x*v2.x + v1.y*v2.y;
}

template<typename T>
T dot(const vec3<T>& v1, const vec3<T>& v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template<typename T>
T distance2(const vec2<T>& v1, const vec2<T>& v2)
{
    return util::pow2(v1.x - v2.x) + util::pow2(v1.y - v2.y);
}

template<typename T>
T distance2(const vec3<T>& v1, const vec3<T>& v2)
{
    return util::pow2(v1.x - v2.x) + util::pow2(v1.y - v2.y) + util::pow2(v1.z - v2.z);
}

template<typename T>
vec2<T> normalize(const vec2<T>& v)
{
    auto length = v.length();
    return { v.x / length, v.y / length };
}
template<typename T>
vec3<T> normalize(const vec3<T>& v)
{
    auto length = v.length();
    return { v.x / length, v.y / length, v.z / length };
}

/*
 * Reflect about a normalized axis vector
 */
template<typename T>
vec2<T> reflectNormalized(const vec2<T>& v, const vec2<T>& axis)
{
    return axis*dot(v, axis)*2 - v;
}
template<typename T>
vec3<T> reflectNormalized(const vec3<T>& v, const vec3<T>& axis)
{
    return axis*dot(v, axis)*2 - v;
}

using fvec2 = vec2<float>;
using fvec3 = vec3<float>;