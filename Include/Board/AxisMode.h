#pragma once

#include "Base/Vector3.h"

/** \brief Axis mode for board operation. */
typedef enum GSdkAxisMode {
	/**
	* \brief Offset depends from axes oﬀset from tared orientation.
	* It limites offset clamping to real rotation an between -90 and
	* +90 degrees.*/
	GSdkAxisModeProjection = 0,

	/** \brief Rotation of axis is assuming to be rotation in plane orthogonal to 'realAxis'. */
	GSdkAxisModeCircular,

	/** \brief Rotation is similar to Euler angles around specific axis. */
	GSdkAxisModeEuler,

	/** \brief Rotation is defined using quaternion swing-twist algorithm */
	GSdkAxisModeTwist,
} GSdkAxisMode;

#ifdef __cplusplus  

namespace GSdk {
	namespace Board {
		typedef GSdkAxisMode AxisMode;
	}
}

/** \brief Returns string representation of this value. */
std::string getString(const GSdkAxisMode &value);

#endif
