#include "vec.h"

/*
 * Represents a linear transformation from R^3 into R^3, represented by a 3x3 matrix.
 */
template<typename T>
struct Mat3
{
		// Initialize transformation matrix with column vectors (or equivilently,
	  // for transformed standard basis vectors).
		Mat3(vec3<T> e1, vec3<T> e2, vec3<T> e3)
			: m_e1(e1), m_e2(e2), m_e3(e3)
		{}

		vec3<T> transform(const vec3<T>& vec)
		{
			return vec.x*m_e1 + vec.y*m_e2 + vec.z*m_e3;
		}

		vec3<T> m_e1, m_e2, m_e3; // Transformed basis vectors (i.e. column vectors of matrix)
};

// Returns the matrix representing the composition of m1 and m2 (this is really
// just matrix multiplication).
template<typename T>
Mat3<T> compose(const Mat3<T>& m1, const Mat3<T>& m2)
{
	// Simple, but inefficient.
	return
	{
		m1.transform(m2.transform({1, 0, 0})),
		m1.transform(m2.transform({0, 1, 0})),
		m1.transform(m2.transform({0, 0, 1}))
	};
}

template<typename T>
Mat3<T> operator*(const Mat3<T>& m1, const Mat3<T>& m2)
{
	return compose(m1, m2);
}

using fmat3 = Mat3<float>;

fmat3 translate(fvec3 )
