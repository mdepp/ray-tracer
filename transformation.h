/*
 * File: transformation.h
 * Authors: Alexander Epp (1487716) and Mitchell Epp (1498821)
 * Project: CMPUT274 Final Project
 * Description: Contains a class for 3D transformations
 */

#include "vec.h"

/*
 * Trans3 is a transformation from R^3 -> R^3 represented by a 4x4 matrix
 * (this is using homogeneous coordinates I believe, but I'm not entirely clear
 * on the terminology). This supports all linear tranformations, plus translation,
 * linear perspective, etc.
 * 
 * Most operations (e.g. composition) are very inefficient, but are very simple
 * to write, understand, and debug.
 */
template<typename T>
struct Trans3
{
	// Initialize transformation matrix with column vectors (or equivalently,
	// for transformed standard basis vectors).
    Trans3(vec4<T> e1, vec4<T> e2, vec4<T> e3, vec4<T> e4) noexcept
        : m_e1(e1), m_e2(e2), m_e3(e3), m_e4(e4)
    {}
    Trans3(vec3<T> e1, vec3<T> e2, vec3<T> e3) noexcept
        : m_e1(e1), m_e2(e2), m_e3(e3), m_e4(0, 0, 0, 1)
    {}
    // Initialize transformation as identity by default
    Trans3() noexcept
        : Trans3({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}) {}

    
    vec4<T> transform(const vec4<T>& vec) const noexcept
    {
        return { m_e1*vec.x + m_e2*vec.y + m_e3*vec.z + m_e4*vec.w };
    }
	vec3<T> transform(const vec3<T>& vec) const noexcept
	{
        vec4<T> transformed = transform(vec4<T>(vec, 1));
        return vec3<T>{transformed / transformed.w};
	}

	vec4<T> m_e1, m_e2, m_e3, m_e4; // Transformed basis vectors (i.e. column vectors of matrix)
};

// Returns the transformation representing the composition of m1 and m2 (this is
// really just matrix multiplication).
template<typename T>
Trans3<T> compose(const Trans3<T>& m1, const Trans3<T>& m2)
{
	// Simple, but inefficient.
	return
	{
		m1.transform(m2.transform({1, 0, 0, 0})),
		m1.transform(m2.transform({0, 1, 0, 0})),
		m1.transform(m2.transform({0, 0, 1, 0})),
        m1.transform(m2.transform({0, 0, 0, 1}))
	};
}

template<typename T>
Trans3<T> operator*(const Trans3<T>& m1, const Trans3<T>& m2)
{
    return compose(m1, m2);
}
template<typename T>
vec4<T> operator*(const Trans3<T>& trans, const vec4<T>& vec)
{
    return trans.transform(vec);
}
template<typename T>
vec3<T> operator*(const Trans3<T>& trans, const vec3<T>& vec)
{
    return trans.transform(vec);
}

using ftrans3 = Trans3<float>;

// Convert position vector to translation matrix
template<typename T>
Trans3<T> translate(vec3<T> vec)
{
    return
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {vec.x, vec.y, vec.z, 1}
    };
}