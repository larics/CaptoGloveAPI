#pragma once

#include "Base/Vector3.h"

#ifdef __cplusplus  
extern "C" {
#endif  
	/**
	* \brief Settings for customization of acceleration axis movement.
	*/
#pragma pack (push, 1)
	typedef struct {
		/** \brief Is this axis link enabled.*/
		bool isEnabled;

		/**
		* \brief This motion axis, vector of floats.
		* Relates to physical axis orientation of device.
		*/
		GSdkVector3f realAxis;
	} GSdkAccelAxisInfo;
#pragma pack (pop)

	/** \brief Makes this axis info with components */
	GSdkAccelAxisInfo GSdkAccelAxisInfoMakeComponents(bool isEnabled, GSdkVector3f realAxis);

#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus  

namespace GSdk {
	namespace Board {
		using namespace Base;

		typedef GSdkAccelAxisInfo AccelAxisInfo;

		/** \brief Makes this axis info with components */
		AccelAxisInfo getAccelAxisInfo(const bool& isEnabled, const Vector3f& realAxis);

		/** \brief Provides empty info */
		AccelAxisInfo getEmptyAccelAxisInfo();
	}
}
/** \brief Returns string representation of this boost info. */
std::string getString(const GSdkAccelAxisInfo &value);

#endif