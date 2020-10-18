#pragma once

#include "Base/AssemblyTypes.h"

#ifdef __cplusplus
#include <string>
#endif

GSDK_EXTERN_BEGIN

/**
* \brief Structure, describing tag and calibration value for given sensor
*/
#pragma pack (push, 1)
typedef struct {
	/**
	* \brief Minimum allowed conductivity value of this sensor. It lies in
	* range from 0 to some physical value Siemens/meter.
	*/
	float minValue;

	/**
	* \brief Maximum allowed conductivity value of this sensor. It lies in
	* range from 0 to some physical value Siemens/meter.
	*/
	float maxValue;

	/**
	* \brief Hysteresis value, in bounds of which device should ignore
	* conductivity changes. Expressed in Siemens/meter.
	*/
	float hysteresisValue;

	/**
	* \brief Time after which device should take a decision about sensor value
	* change. Expressed in seconds.
	*/
	float confidenceTime;

	/**
	* \brief Factor for filter, expressed from 0 to 1. Less factor leads to
	* more slow reaction time but to greater smoothness.
	*/
	float filterFactor;

	/**
	* \brief User tag for this sensor. Can be any value, representing sensor
	* predestination.
	*/
	unsigned char tag;
} GSdkSensorDescriptor;
#pragma pack (pop)

/**
* \brief Predefined settings, used to configure this sensor.
*/
typedef enum {
	/** \brief Fast response but low smoothness. */
	GSdkSensorConfigurationFast = 0,

	/** \brief Balanced between performance and smoothness. */
	GSdkSensorConfigurationBalanced = 1,

	/** \brief Low response speed but good smoothness */
	GSdkSensorConfigurationSmooth = 3,
} GSdkSensorConfiguration;

/** \brief Determines where or no 2 key sensor descriptors are equal */
GSDK_API bool __cdecl GSdkSensorDescriptorEqualToSensorDescriptor(const GSdkSensorDescriptor *lhs, const GSdkSensorDescriptor *rhs);

/** \brief Creates descriptor with predefined settings. */
GSDK_API GSdkSensorDescriptor __cdecl GSdkSensorDescriptorMakePredefined(const GSdkSensorConfiguration configuration);

GSDK_EXTERN_END

#ifdef __cplusplus

namespace GSdk {
	namespace Board {
#pragma pack (push, 1)
		struct SensorDescriptor : public GSdkSensorDescriptor {
			/** \brief Is this sensor calibrated or no */
			bool isCalibrated() const;

			/**
			* \brief Provides relative value, depending from inversion state of sensor.
			* This means that for inverted state minimum allowed value should
			* be more that maximum one.
			* \param value The value for finding frowing relative one
			* \param inverted Is this value inverted
			*/
			float growingRelativeValue(const float& value, const bool& inverted) const;

			/** \brief Static value with some predefined configuration. */
			static SensorDescriptor predefined(const GSdkSensorConfiguration& configuration);

			/** \brief Constructs empty trigger info */
			SensorDescriptor();

			/** \brief Copies low level trigger info */
			SensorDescriptor(const GSdkSensorDescriptor& value);

			/** Is this triggger info equalts to another one */
			bool operator==(const SensorDescriptor& another) const;
		};
#pragma pack (pop)
	}
}

/** \brief Description of this sensor descriptor */
std::string getString(const GSdkSensorDescriptor &value);

#endif
