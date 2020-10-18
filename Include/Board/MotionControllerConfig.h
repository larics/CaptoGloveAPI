#pragma once

#include <stdint.h>

#ifdef __cplusplus
#include <string>
#endif

#ifdef __cplusplus  
extern "C" {
#endif  
	/**
	* \brief Structure, describing motion controller (acceleration, hysteresis, and so on)
	*/
#pragma pack (push, 1)
	typedef struct {
		uint32_t accelTriggerPeriod;
		float accelTriggerHysteresis;
		uint32_t inputAxisTriggerPeriod;
		float inputAxisTriggerHysteresis;
		uint32_t barometerTriggerPeriod;
		float barometerTriggerHysteresis;
		float barometerMultiplier;
	} GSdkMotionControllerConfig;
#pragma pack (pop)

#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus  

namespace GSdk {
	namespace Board {
		typedef GSdkMotionControllerConfig MotionControllerConfig;

		/** \brief Provides empty info */
		MotionControllerConfig getEmptyMotionControllerConfig();
	}
}

/** \brief Returns string representation of this boost info. */
std::string getString(const GSdkMotionControllerConfig &value);

#endif