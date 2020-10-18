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
	} GSdkInputAxisInfo;
#pragma pack (pop)

	/**
	* \brief Initializes a new instance of the <see cref="InputAxisInfo"/> struct.
	* @param isEnabled if set to <c>true</c> [is enabled].
	* @param axisMode An axis mode to set
	* @param realAxis This motion axis, vector of floats.
	*/
	GSdkInputAxisInfo GSdkInputAxisInfoMakeComponents(bool isEnabled, unsigned char axisMode, GSdkVector3f realAxis);
#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus 

namespace GSdk {
	namespace Board {
		using namespace Base;

		typedef GSdkInputAxisInfo InputAxisInfo;

		/** \brief Makes this axis info with components */
		InputAxisInfo getInputAxisInfo(const bool& isEnabled, const AxisMode& axisMode, const Vector3f& realAxis);

		/** \brief Provides empty info */
		InputAxisInfo getEmptyInputAxisInfo();
	}
}
/** \brief Returns string representation of this boost info. */
std::string getString(const GSdkInputAxisInfo &value);

#endif
