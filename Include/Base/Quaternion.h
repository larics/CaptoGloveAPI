#pragma once

#include "Base/Vector3.h"
#include "Base/VectorTypes.h"

#ifdef __cplusplus
#include <sstream>
#include <string>
#endif

#ifdef __cplusplus  
extern "C" {
#endif  
	/** \brief Representation of rotation components. */
#pragma pack (push, 1)
	typedef struct {
		float x;
		float y;
		float z;
		float w;
	} GSdkQuaternionf;
#pragma pack (pop)

	/** \brief Initializes vector with given components */
	GSdkQuaternionf GSdkQuaternionfMakeComponents(float x, float y, float z, float w);

	/** \brief Normializes given quaternion */
	GSdkQuaternionf GSdkQuaternionfNormalize(GSdkQuaternionf value);

	/** \brief Conjugates given quaternion */
	GSdkQuaternionf GSdkQuaternionfConjugate(GSdkQuaternionf value);

	/** \brief Creates quaternion from euler angles */
	GSdkQuaternionf GSdkQuaternionfMakeEuler(GSdkVector3f euler);

	/** \brief Covnerts given quaternion to euler angles */
	GSdkVector3f GSdkQuaternionfGetEuler(GSdkQuaternionf value);
#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus 

namespace GSdk {
	namespace Base {
		typedef GSdkQuaternionf Quaternionf;

		/** \brief Makes quaternion from components */
		Quaternionf getQuaternion(const float& x, const float& y, const float& z, const float& w);

		/** \brief Creates quaternion from euler angles */
		Quaternionf fromEuler(const Vector3f& euler);

		/** \brief Normializes given quaternion */
		Quaternionf normalize(const Quaternionf& value);

		/** \brief Conjugates given quaternion */
		Quaternionf conjugate(const Quaternionf& value);

		/** \brief Covnerts given quaternion to euler angles */
		Vector3f getEuler(const Quaternionf& value);

		/**
		Decompose the rotation on to 2 parts.
		1. Twist - rotation around the "direction" vector
		2. Swing - rotation around axis that is perpendicular to "direction" vector
		The rotation can be composed back by `rotation` = `swing` * `twist`.

		Has singularity in case of swing_rotation close to 180 degrees rotation.
		if the input quaternion is of non-unit length, the outputs are non-unit as well
		otherwise, outputs are both unit.

		And the long answer and derivation of this code can be found here
		http://www.euclideanspace.com/maths/geometry/rotations/for/decomposition/
		*/
		void decompose(const Quaternionf quaternion, const Vector3f& direction, Quaternionf& swing, Quaternionf& twist);
	}
}

/** \brief Provides string representation of given quaternion */
std::string getString(const GSdkQuaternionf &value);

/** \brief Multiplies quaternion to another quaternion. */
GSdkQuaternionf operator * (const GSdkQuaternionf &lhs, const GSdkQuaternionf &rhs);

#endif
