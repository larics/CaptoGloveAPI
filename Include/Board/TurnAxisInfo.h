#pragma once

#include "Base/Vector3.h"
#include "Board/AxisMode.h"

#ifdef __cplusplus  
extern "C" {
#endif  
	/**
	* \brief Settings for customization of input axis axis movement.
	*/
#pragma pack (push, 1)
	typedef struct {
		/** \brief Is this axis link enabled.*/
		bool isEnabled;

		/** \brief Axis mode. For description @see AxisMode. */
		unsigned char axisMode;

		/**
		* \brief This motion axis, vector of floats.
		* Relates to physical axis orientation of device.
		*/
		GSdkVector3f realAxis;
	} GSdkTurnAxisInfo;
#pragma pack (pop)

	/**
	* \brief Initializes a new instance of the <see cref="TurnAxisInfo"/> struct.
	* @param isEnabled if set to <c>true</c> [is enabled].
	* @param axisMode An axis mode to set
	* @param realAxis This motion axis, vector of floats.
	*/
	GSdkTurnAxisInfo GSdkTurnAxisInfoMakeComponents(bool isEnabled, unsigned char axisMode, GSdkVector3f realAxis);
#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus 

namespace GSdkCxxBoard {
	using namespace GSdkCxxBase;

	typedef GSdkTurnAxisInfo TurnAxisInfo;

	/** \brief Makes this axis info with components */
	TurnAxisInfo getTurnAxisInfo(const bool &isEnabled, const AxisMode &axisMode, const Vector3f &realAxis);

	/** \brief Provides empty info */
	TurnAxisInfo getEmptyTurnAxisInfo();
}
/** \brief Returns string representation of this boost info. */
std::string getString(const GSdkTurnAxisInfo &value);

#endif
