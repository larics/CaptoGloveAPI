#pragma once

#ifdef __cplusplus
#include <string>
#include <math.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <limits>
#else
#include <stdbool.h>
#endif

#ifdef __cplusplus  
extern "C" {
#endif  
	/**
	* \brief Three dimensional vector structure.
	*  It can describe either spatial cartesian point or Euler
	* angles or something else.
	*/
#pragma pack (push, 1)
	typedef struct {
		float x;
		float y;
		float z;
	} GSdkVector3f;
#pragma pack (pop)

	/** \brief Spatial direction of given vector */
	typedef enum {
		/** \brief Forward diraction, aka Z positive */
		GSdkVectorDirectionForward,

		/** \brief Backward diraction, aka Z positive */
		GSdkVectorDirectionBackward,

		/** \brief Left diraction, aka X positive */
		GSdkVectorDirectionLeft,

		/** \brief Right diraction, aka X negative */
		GSdkVectorDirectionRight,

		/** \brief Right diraction, aka Y positive */
		GSdkVectorDirectionDown,

		/** \brief  Up diraction, aka Y negative */
		GSdkVectorDirectionUp,
	} GSdkVectorDirection;

	/** \brief Cartesian axis components of 3 dimensional vector */
	typedef enum {
		GSdkCartesianAxisX, 
		GSdkCartesianAxisY, 
		GSdkCartesianAxisZ,
	} GSdkCartesianAxis;

	/** \brief Construct identity */
	extern GSdkVector3f const GSdkVector3fZero;

	/** \brief Initializes vector with given components */
	GSdkVector3f GSdkVector3fMakeComponents(float x, float y, float z);

	/** \brief Provides assotiated vector with given direction */
	GSdkVector3f GSdkVector3fMakeDirection(GSdkVectorDirection direction);

	/** \brief Looks for most collinear axis for this vector */
	GSdkVectorDirection GSdkVector3fGetDirection(GSdkVector3f vector);

	/** \brief Magnitude of the vector */
	float GSdkVector3fGetMagnitude(GSdkVector3f vector);

	/** @brief validity of vector  */
	bool GSdkVector3fIsNonNull(GSdkVector3f vector);

	/**
	* @brief Produces dot product of two vectors.
	* @param lhs First component of dot product
	* @param rhs Second component of dot product
	* @return Single value, dot product between lhs and rhs
	*/
	float GSdkVector3fDot(GSdkVector3f lhs, GSdkVector3f rhs);

	/** \brief Cross product of the two Vector3, outputs a Vector3 */
	GSdkVector3f GSdkVector3fCross(GSdkVector3f lhs, GSdkVector3f rhs);

	/**
	* \brief Angle between two Vectors
	* @param lhs first vector of the pair, where the angle is calculated
	* @param rhs second vector of the pair, where the angle is calculated
	* @return angle between the two vectors, radians
	*/
	float GSdkVector3fAngle(GSdkVector3f lhs, GSdkVector3f rhs);

	/** \brief Appends second vector to components of first one */
	GSdkVector3f GSdkVector3fAdd(GSdkVector3f lhs, GSdkVector3f rhs);

	/** \brief Ubstracts second vector from components of first one */
	GSdkVector3f GSdkVector3fSubstract(GSdkVector3f lhs, GSdkVector3f rhs);

	/** \brief Multiplyes components of first vector to components of second one */
	GSdkVector3f GSdkVector3fMultiply(GSdkVector3f lhs, GSdkVector3f rhs);

	/** \brief Divides components of first vector to components of second one */
	GSdkVector3f GSdkVector3fDivide(GSdkVector3f lhs, GSdkVector3f rhs);

	/**
	* \brief Projects an instance of the vector onto another (3D)
	* @param lhs vector on which the projection is done
	* @param rhs projecting vector
	* @return new projected vector, whos direction is equal to lhs
	* and lenght is defined by dot product between lhs and rhs.
	*/
	GSdkVector3f GSdkVector3fProject(GSdkVector3f lhs, GSdkVector3f rhs);

	/**
	* \brief Calculates the distance between two vectors
	* @param lhs vector to calculate the distance from
	* @param rhs vector to calculate the distance to
	* @return distance between the two vectors
	*/
	float GSdkVector3fDistance(GSdkVector3f lhs, GSdkVector3f rhs);

	/**
	* Normalizes an instance of the vector
	* @return reference to a new normalized Vector3
	*/
	GSdkVector3f GSdkVector3fNormalize(GSdkVector3f vector);

	/** \brief Inverts an inverted instance of the vector */
	GSdkVector3f GSdkVector3fInvert(GSdkVector3f vector);
#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus  

namespace GSdk {
	namespace Base {
		typedef GSdkVector3f Vector3f;
		typedef GSdkVectorDirection VectorDirection;

		/** \brief Initializes vector with given components */
		Vector3f getVector(const float& x, const float& y, const float& z);

		/** \brief Provides assotiated vector with given direction */
		Vector3f getVector(const VectorDirection& direction);

		/** \brief Looks for most collinear axis for this vector */
		VectorDirection getDirection(const Vector3f& vector);

		/** \brief Magnitude of the vector */
		float getMagnitude(const Vector3f& vector);

		/** @brief validity of vector  */
		bool isNonNull(const Vector3f& vector);

		/** 
		* @brief Produces dot product of two vectors.
		* @param lhs First component of dot product
		* @param rhs Second component of dot product
		* @return Single value, dot product between lhs and rhs
		*/
		float dot(const Vector3f& lhs, const Vector3f& rhs);

		/** \brief Cross product of the two vectors */
		Vector3f cross(const Vector3f& lhs, const Vector3f& rhs);

		/**
		* \brief Angle between two Vectors
		* @param lhs first vector of the pair, where the angle is calculated
		* @param rhs second vector of the pair, where the angle is calculated
		* @return angle between the two vectors, radians
		*/
		float angle(const Vector3f& lhs, const Vector3f& rhs);

		/**
		* \brief Projects an instance of the vector onto another (3D)
		* @param lhs vector on which the projection is done
		* @param rhs projecting vector
		* @return new projected vector, whos direction is equal to lhs 
		* and lenght is defined by dot product between lhs and rhs.
		*/
		Vector3f project(const Vector3f& lhs, const Vector3f& rhs);

		/**
		* \brief Calculates the distance between two vectors
		* @param lhs vector to calculate the distance from
		* @param rhs vector to calculate the distance to
		* @return distance between the two vectors
		*/
		float distance(const Vector3f& lhs, const Vector3f& rhs);

		/**
		* Normalizes an instance of the vector
		* @return reference to a new normalized Vector3
		*/
		Vector3f normalize(const Vector3f& vector);

		/** \brief Inverts an inverted instance of the vector */
		Vector3f invert(const Vector3f& vector);
	}
}

/** \brief Prints this value to string */
std::string getString(const GSdkVector3f &value);

/** \brief Prints this value to string */
std::string getString(const GSdkVectorDirection &value);

/** \brief Appends second vector to components of first one */
GSdkVector3f operator + (const GSdkVector3f &lhs, const GSdkVector3f &rhs);

/** \brief Ubstracts second vector from components of first one */
GSdkVector3f operator - (const GSdkVector3f &lhs, const GSdkVector3f &rhs);

/** \brief Multiplyes components of first vector to components of second one */
GSdkVector3f operator * (const GSdkVector3f &lhs, const GSdkVector3f &rhs);

/** \brief Divides components of first vector to components of second one */
GSdkVector3f operator / (const GSdkVector3f &lhs, const GSdkVector3f &rhs);

#endif
