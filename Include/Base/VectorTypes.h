
#pragma once

#include "Base/Quaternion.h"

#ifdef __cplusplus
#include <string>
#include <math.h>
#include <sstream>
#include <iostream>
#include <vector>
#endif

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifdef __cplusplus

namespace GSdk {
	namespace Base {
		/** \brief Axis of vector/quaternion */
		typedef enum Axis {
			X,
			Y,
			Z,
			W,
		} Axis;

		/**
		* \brief Add other angle to this angle
		* \param angle this angle
		* \param otherAngle another angle
		* \return angular sum
		*/
		template<typename T>
		T angularSum(const T& angle, const T& otherAngle) {
			return atan2(sin(angle) + sin(otherAngle), cos(angle) + cos(otherAngle));
		}

		/**
		* \brief Interpolation result of sum of 2 angles
		* \param angle this angle
		* \param anotherAngle another angle
		* \param delta Delta (0 - 1) between angles
		* \return angular interpolation result
		*/
		template<typename T>
		T angularInterp(const T& angle, const T& anotherAngle, const T& delta) {
			double x0 = cos(angle), y0 = sin(angle);
			double x1 = cos(anotherAngle), y1 = sin(anotherAngle);
			double x = x0 * (1 - delta) + x1 * delta;
			double y = y0 * (1 - delta) + y1 * delta;
			return atan2(y, x);
		}

		/**
		* \brief Reduces angle to given range
		* \param value reducing value
		* \param lower lower bound
		* \param upper bound
		* \return Reduced angle
		*/
		template<typename T>
		T angularReduce(const T& value, const T& lower = 0, const T & upper = 2 * M_PI) {
			T aValue = value;
			aValue -= lower; // Adjust to zero
			T rangeSize = upper - lower;
			if (rangeSize == 0) { return upper; } // avoid dividing by 0
			return aValue - (rangeSize * static_cast<T>(floor(aValue / rangeSize))) + lower;
		}

		/**
		* \brief constrain a value to lie between two further values
		* \param x Specify the value to constrain.
		* \param minVal Specify the lower end of the range into which to constrain x.
		* \param maxVal Specify the upper end of the range into which to constrain x.
		* \return the value of x constrained to the range minVal to maxVal.
		*/
		template<typename T>
		T clamp(const T& x, const T& minVal, const T& maxVal) {
			T newValue = x;
			if (newValue < minVal) {
				newValue = minVal;
			}
			else if (newValue > maxVal) {
				newValue = maxVal;
			}
			return newValue;
		}
	}
}

#endif
