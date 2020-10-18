#pragma once

#include "Base/AssemblyTypes.h"
#include "Base/Event.h"

#ifdef __cplusplus
#include <functional>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <iomanip>
#endif

#ifdef __cplusplus  
extern "C" {
#endif  
	/** \brief Error domain for board errors */
	extern const int GSdkGSdkBoardDomain;

	/** \brief Possible error codes for result */
	enum GSdkBoardError {
		GSdkBoardErrorServiceNotDiscovered = 1,
		GSdkBoardErrorIncorrectTimeslots = 2,
		GSdkBoardErrorBoardDisconnected = 3,
		GSdkBoardErrorServiceNotFound = 4,
		GSdkBoardErrorCharacteristicNotDiscovered = 5,
		GSdkBoardErrorCharacteristicNotFound = 6,
		GSdkBoardErrorCharacteristicNotRead = 7,
		GSdkBoardErrorDescriptorNotRead = 8,
		GSdkBoardErrorDescriptorNotWrite = 9,
		GSdkBoardErrorSubscriptionFailed = 10,
		GSdkBoardErrorUnsubscriptionFailed = 11,
		GSdkBoardErrorCommandStreamNotStarted = 12,
		GSdkBoardErrorCommandStreamNotStoped = 13,
		GSdkBoardErrorIncorrectRegisterData = 14,
		GSdkBoardErrorInvalidParameter = 15,
	};
#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus
namespace GSdk {
	namespace Board {
		using namespace Base;

		/** \brief Change of the pripheral property */
		struct BoardProperty final {
		public:
			/** \brief Battery level. Returns int * as value with size of 4 bytes. */
			static const std::string batteryLevel;

			/** \brief Battery voltage. Returns float * as value with size of 4 bytes. */
			static const std::string batteryVoltage;

			/** \brief Temperature. Returns float * as value with size of 4 bytes. */
			static const std::string temperature;

			/** \brief Firmware version. Returns UTF8 null terminated string as value. */
			static const std::string firmwareVersion;

			/** \brief Serial number. Returns UTF8 null terminated string as value. */
			static const std::string serialNumber;

			/**
			* \brief Board sensor descriptor, which allows you to configure response traits.
			*
			* \details For retrieving sensors descriptors you should request it via keypath
			* "sensorDescriptor.{index}". Every sensor descriptor has size of GSdkSensorDescriptor.
			*/
			static const std::string sensorDescriptor;

			/** \brief Magnetormeter state. Returns uint8_t * as value with size of 1 bytes. */
			static const std::string magnetismUsed;

			/** \brief Is gyro bias used for filtering. Returns uint8_t * as value with size of 1 bytes. */
			static const std::string autoGyroBiasUsed;

			static const std::string autoSleepDisabled;

			/** \brief Should gyro bias be calculated while taring. Returns uint8_t * as value with size of 1 bytes. */
			static const std::string autoGyroBiasTared;

			/** \brief Stream timeslots. It has size of GSdkStreamTimeslots */
			static const std::string streamTimeslots;

			/** \brief Indicator color property. Returns uint8_t x 3 as value with size of 3 bytes. */
			static const std::string indicatorColor;

			/** \brief User info property. Returns byte * as value with size of userInfo. */
			static const std::string userInfo;

			static const std::string motionControllerConfig;

			/**
			* \brief Board magnetometer calibration parameters structure.
			*
			* \details This property is typically used for asjusting calibration process
			* for magnetometer. Output value has size of CalibrationConfig.
			*/
			static const std::string magnetometerCalibration;

			/**
			* \brief Board accelerometer calibration parameters structure.
			*
			* \details This property is typically used for asjusting calibration process
			* for accelerometer. Output value has size of CalibrationConfig.
			*/
			static const std::string accelerometerCalibration;

			/**
			* \brief Board gyro calibration parameters structure.
			*
			* \details This property is typically used for asjusting calibration process
			* for gyro. Output value has size of CalibrationConfig.
			*/
			static const std::string gyroCalibration;

			/**
			* \brief Deep board IMU configuration parameters
			*
			* \details After setting this property you should either reboot board or call
			* ResetImuFilter method.
			*/
			static const std::string imuConfig;

			/** \brief Emulation state. Returns uint8_t * as value with size of 1 bytes. */
			static const std::string emulationState;

			/** \brief Emulation modes. It has size of GSdkEmulationModes */
			static const std::string emulationModes;

			/**
			* \brief Board sensor trigger - an action firing on sensor bending/pushing.
			*
			* \details For reteiving specific sensor trigger you should request it via
			* keypath "sensorTrigger.{sensor}.{trigger}". Every sensor has size of GSdkKeyTrigger.
			*/
			static const std::string sensorTrigger;

			/**
			* \brief Board mouse motion link. Configuration for HID mouse movement.
			*
			* \details For getting specific link value you should request it via keypath
			* "mouseMotionLink.{index}". Every sensor has size of BoardMotionLink.
			*/
			static const std::string mouseMotionLink;

			/**
			* \brief Board joystick motion link. Configuration for HID joystick movement.
			*
			* \details For getting specific link value you should request it via keypath
			* "joystickMotionLink.{index}". Every sensor has size of BoardMotionLink.
			*/
			static const std::string joystickMotionLink;

			/**
			* \brief Board boost axis info. Configuration for HID key firing on acceleration.
			*
			* \details For getting specific boost info value you should request it via
			* keypath "accelAxisInfo.{index}". Every sensor has size of AccelAxisInfo.
			*/
			static const std::string accelAxisInfo;

			/**
			* \brief Board boost trigger.
			*
			* \details For getting specific boost trigger value you should request it via
			* keypath "accelTrigger.{axis}.{trigger}". Every sensor has size of KeyTrigger.
			*/
			static const std::string accelTrigger;

			/**
			* \brief Board turn axis info. Configuration for HID key firing on rotation.
			*
			* \details For getting specific turn info value you should request it via
			* keypath "inputAxisInfo.{index}". Every sensor has size of InputAxisInfo.
			*/
			static const std::string inputAxisInfo;

			/**
			* \brief Board turn trigger.
			*
			* \details For getting specific turn trigger value you should request it via
			* keypath "inputAxisTrigger.{axis}.{trigger}". Every sensor has size of KeyTrigger.
			*/
			static const std::string inputAxisTrigger;

			/**
			* \brief Board barometer trigger.
			*
			* \details For getting specific barometer trigger value you should request it via
			* keypath "barometerTrigger.{index}". Every sensor has size of KeyTrigger.
			*/
			static const std::string barometerTrigger;
		};

		/** \brief Board templated error maker */
		template<typename Payload>
		Result<Payload> makeResult(
			const GSdkBoardError& status,
			const std::string& description = "",
			const std::shared_ptr<Payload>& payload = std::shared_ptr<Payload>()) {
			return Result<Payload>(status, GSdkGSdkBoardDomain, description, payload);
		};

		/** Board void error maker */
		Result<void> makeEmptyResult(const GSdkBoardError& status, const std::string& description = "");
	}
}
#endif
