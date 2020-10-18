
#pragma once

#include "Base/VectorTypes.h"
#include "Base/Vector3.h"

#ifdef __cplusplus  
extern "C" {
#endif  
	/** \brief Matrix axis order */
	typedef enum {
		XYZ = 0,
		YZX,
		ZXY,
		ZYX,
		YXZ,
		XZY,
	} GSdkMatrixOrder;

	/**
	* \brief Three space matrix class.
	* \details Binary format is column-major. Coordinate system is right-handed,
	* rotation Order YXZ (first heading, then attitude, then bank).
	*/
#pragma pack (push, 1)
	typedef struct {
		float m00, m01, m02;
		float m10, m11, m12;
		float m20, m21, m22;
	} GSdkMatrix3f;
#pragma pack (pop)

	/** \brief Construct identity */
	extern GSdkMatrix3f const GSdkMatrix3fIdentity;

#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus  

namespace GSdk {
	namespace Base {
		typedef GSdkMatrix3f Matrix3f;

		/** \brief Validates componens of current matrix. */
		bool validate(const Matrix3f& matr);

		/** \brief Finds euler angles from given rotation matrix. */
		Vector3f getEuler(const Matrix3f& matr, const GSdkMatrixOrder& order);

		/** \brief Finds matrix from given rotation euler angles. */
		Matrix3f getMatrix(const Vector3f& vector, const GSdkMatrixOrder& order);
	}
}

/** \brief Generates string representation for current matrix */
std::string getString(const GSdkMatrix3f &value);

/** \brief Multiplies matrix to vector. */
GSdkVector3f operator * (const GSdkMatrix3f &lhs, const GSdkVector3f &rhs);

/** \brief Multiplies matrix to another matrix. */
GSdkMatrix3f operator * (const GSdkMatrix3f &lhs, const GSdkMatrix3f &rhs);

/** \brief Compares 2 matrices */
bool operator == (const GSdkMatrix3f &lhs, const GSdkMatrix3f &rhs);

#endif