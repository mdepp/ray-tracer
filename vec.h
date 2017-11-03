/*
 * Note: This class was based in part on code used in our project 2 part 1 submission
 */

#pragma once

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
	vec2<T> operator - (const vec2<T>& other) const
	{
		return vec2<T>(x - other.x, y - other.y);
	}
	vec2<T> operator + (const vec2<T>& other) const
	{
		return vec2<T>(x + other.x, y + other.y);
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
        T z, a;
    };

    vec2() : x(0), y(0), z(0) {}
    vec2(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}    

    bool operator == (const vec2<T>& other) const noexcept
	{
		return x == other.x && y == other.y && z == other.z;
	}
	bool operator != (const vec2<T>& other) const noexcept
	{
		return x != other.x || y != other.y || z != other.z;
	}
	vec2<T> operator / (T n) const noexcept
	{
		return vec3<T>(x / n, y / n, z / n);
	}
	vec2<T> operator - (const vec2<T>& other) const noexcept
	{
		return vec3<T>(x - other.x, y - other.y, z - other.z);
	}
	vec2<T> operator + (const vec2<T>& other) const noexcept
	{
		return vec3<T>(x + other.x, y + other.y, z + other.z);
	}
};