
#pragma once

#include "Matrix3.h"
#include "Vector3.h"
#include "VectorTypes.h"

#ifdef __cplusplus
namespace GSdk {
	namespace Base {
		/**
		* @brief Calculates the spherical coordinates for given vector.
		* @return spherical coordinate of the vector
		*/
		Vector3f sphericalWithCartesian(const Vector3f& vect);

		Vector3f cartesianWithSpherical(const Vector3f& vect);

		/**
		* \brief Reduces given value between bounds. This means that value,
		* will lie in bounds from minimum to maximum with offset, defining
		* by integer part of difference value and minimum bound.
		*
		* @param v: A value to reduce.
		* @param x: Minimum bound of value
		* @param y: Maximum bound of value
		* @return: The reduced of floating value
		*/
		template<typename T> T reduce(const T& v, const T& x, const T& y) {
			T value = v - x;
			T range = y - x;
			return value - range * floor(value / range) + x;
		}
	}
}
#endif
