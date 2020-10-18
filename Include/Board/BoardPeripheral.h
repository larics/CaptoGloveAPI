#pragma once

#include "Board/BoardColor.h"
#include "Board/FirmwareVersion.h"
#include "Board/BoardMotionLink.h"
#include "Board/BoardStream.h"
#include "Board/BoardTypes.h"
#include "Board/CalibrationConfig.h"
#include "Board/EmulationModes.h"
#include "Board/ImuConfig.h"
#include "Board/MotionControllerConfig.h"
#include "Board/KeyTrigger.h"
#include "Board/SensorDescriptor.h"
#include "Board/StreamTimeslots.h"
#include "Board/AccelAxisInfo.h"
#include "Board/InputAxisInfo.h"
#include "Base/AssemblyTypes.h"
#include "Base/Quaternion.h"
#include "Base/Vector3.h"
#include "Peripheral/PeripheralBase.h"
#include "Peripheral/PeripheralRegister.h"

#ifdef __cplusplus
#include <string>
#include <memory>

namespace GSdk {
	namespace Board {
		using namespace Peripheral;

		/**
		* \brief Major class for interaction with board.
		*
		* \details Typical scenario of usage is to discover boards from BoardCentral,
		* connect to it, get some basic information like name, firmware, and
		* so on, start properties stream if needed.
		*/
		class BoardPeripheral : public virtual PeripheralBase {
		public:
			/** \brief On new stream data arriving */
			virtual BoardStreamEvent& streamReceived() = 0;

			/** \brief Reboots board. */
			virtual EmptyResult rebootBoard() = 0;

			/** \brief Tares board with current orientation. */
			virtual EmptyResult tare() = 0;

			/** \brief Resets board's kalman filter. */
			virtual EmptyResult resetImuFilter() = 0;

			/**
			* \brief Performs auto-gyroscope calibration. Sensor should remain still while
			* samples are taken.The gyroscope bias will be automatically placed into
			* the bias part of the gyroscope calibration coefficient list. Result of
			* command is returning on calibration finalising.
			*
			* @returns result of gyro calibrating operation
			*/
			virtual EmptyResult calibrateGyro() = 0;

			/**
			* \brief Performs barometer taring. After this operation relative altitude
			* stream value become zero at current point.
			* @returns Result of taring command.
			*/
			virtual EmptyResult tareBarometer() = 0;

			/**
			* \brief Commits all changes to board.
			* @returns Result of command.
			*/
			virtual EmptyResult commitBoard() = 0;

			/**
			* \brief Boots this board to DFU mode.
			* @returns Result of command.
			*/
			virtual EmptyResult bootToDFU() = 0;

			/**
			* \brief Resets board settings to factory ones.
			* @returns Result of command.
			*/
			virtual EmptyResult resetSettings() = 0;

			/**
			* \brief Updates board connection interval, ms. Minimum allowed value is 7.5 ms.
			* @returns Result of command.
			*/
			virtual EmptyResult setConnectionInterval(const float& value) = 0;

			/*
			* \brief Magnetometer state from board. Can be either entirely
			* enabled and taking place in quaternion caluclations, or fully
			* disabled, or with reducing coefficient (inside bad conditions).
			*/
			virtual const WritableRegister<bool>& magnetismUsed() const = 0;

			/** \brief Current status of using bias while IMU filtering */
			virtual const WritableRegister<bool>& autoGyroBiasUsed() const = 0;

			/** \brief Should board tare bias for gyro while taring */
			virtual const WritableRegister<bool>& autoGyroBiasTared() const = 0;

			/** \brief Should board be disabled after hold on interval */
			virtual const WritableRegister<bool>& autoSleepDisabled() const = 0;

			/** \brief Reads battery level from board. Battery level is expressed in percents */
			virtual const ReadableRegister<int>& batteryLevel() const = 0;

			/** \brief Current battery voltage, volts */
			virtual const ReadableRegister<float>& batteryVoltage() const = 0;

			/** \brief Firmware version from board. */
			virtual const FirmwareVersion& firmwareVersion() const = 0;

			/** \brief Serial number, unique identifying this board. */
			virtual const WritableRegister<std::string>& serialNumber() const = 0;

			/** \brief Current board temperature. Expressed in degrees of celcius. */
			virtual const ReadableRegister<float>& temperature() const = 0;

			/* \brief Sensor descriptor for given sensor index. */
			virtual const WritableRegister<SensorDescriptor>& sensorDescriptor(const int& index) const = 0;

			/* \brief Subscription timeslots. More timeslots for given endpoint lead to performance improvement.c*/
			virtual const WritableRegister<StreamTimeslots>& streamTimeslots() const = 0;

			/** \brief LED indicator color */
			virtual const WritableRegister<BoardColor>& indicatorColor() const = 0;

			/** \brief Custom user info, written to the board. Maximum size is 64 bytes. */
			virtual const WritableRegister<std::vector<unsigned char>>& userInfo() const = 0;

			/**
			* \brief Board magnetometer calibration parameters structure.
			*
			* \details This property is typically used for adjusting calibration process
			* for magnetometer.
			*/
			virtual const WritableRegister<CalibrationConfig>& magnetometerCalibration() const = 0;

			/**
			* \brief Board accelerometer calibration parameters structure.
			*
			* \details This property is typically used for asjusting accelerometer process
			* for accelerometer.
			*/
			virtual const WritableRegister<CalibrationConfig>& accelerometerCalibration() const = 0;

			/**
			* \brief Board gyro calibration parameters structure.
			*
			* \details This property is typically used for asjusting accelerometer process
			* for gyro.
			*/
			virtual const WritableRegister<CalibrationConfig>& gyroCalibration() const = 0;

			/** \brief Current IMU config */
			virtual const WritableRegister<ImuConfig>& imuConfig() const = 0;

			/** \brief Current motion controller config */
			virtual const WritableRegister<MotionControllerConfig>& motionControllerConfig() const = 0;

			/**
			* \brief Emulation state for this glove. This is major value, describing
			* sending of HID streams to system. In case of disabling this state
			* board will not behave itself as mouse, keyboard, joystick.
			*/
			virtual const WritableRegister<bool>& emulationState() const = 0;

			/**
			* \brief Emulation modes for this glove. This is defining how is
			* glove will behave exactly as HID device. You can enable
			* any of devices, i.e. joystick, mouse, or keyboard.
			*/
			virtual const WritableRegister<EmulationModes>& emulationModes() const = 0;

			/** \brief Relation between movement and emulating mouse virtual axis. */
			virtual const WritableRegister<BoardMotionLink>& mouseMotionLink(const int& index) const = 0;

			/** \brief Relation between movement and emulating joystick virtual axis. */
			virtual const WritableRegister<BoardMotionLink>& joystickMotionLink(const int& index) const = 0;

			/* \brief Sensor trigger for given sensor index. */
			virtual const WritableRegister<KeyTrigger>& sensorTrigger(const int& sensor, const int& trigger) const = 0;

			/*
			* \brief Returns boost axis info for given index.
			*
			* \param index Index should lie in bounds from 0 to GSDK_BOOST_LINK_COUNT.
			*/
			virtual const WritableRegister<AccelAxisInfo>& accelAxisInfo(const int& index) const = 0;

			/** \brief Boost trigger for given axis.*/
			virtual const WritableRegister<KeyTrigger>& accelTrigger(const int& axis, const int& trigger) const = 0;

			/*
			* \brief Returns boost axis info for given index.
			*
			* \param index Index should lie in bounds from 0 to GSDK_INPUT_AXIS_LINK_COUNT.
			*/
			virtual const WritableRegister<InputAxisInfo>& inputAxisInfo(const int& index) const = 0;

			/** \brief Turn trigger for given axis.	*/
			virtual const WritableRegister<KeyTrigger>& inputAxisTrigger(const int& axis, const int& trigger) const = 0;

			/** \brief Barometer trigger for given index.*/
			virtual const WritableRegister<KeyTrigger>& barometerTrigger(const int& index) const = 0;

			/**
			* \brief Retrieves attached property for parsed path.
			*
			* \details Concrete binary layout of vector depends from type of property.
			* Subclasses should override this method.
			*/
			std::vector<unsigned char> property(const KeyPath& keyPath) const;

			/**
			* \brief Reads attached property for parsed path.
			*
			* \details Subclasses should override this method.
			*/
			EmptyResult readProperty(const KeyPath& keyPath);

			/**
			* \brief Writes attached property for parsed path.
			*
			* \details Subclasses should override this method.
			*/
			EmptyResult writeProperty(const KeyPath& keyPath, const std::vector<unsigned char> prop);
		};

		/** \brief Shared peripheral resource */
		typedef std::shared_ptr<BoardPeripheral> BoardPeripheralPtr;
	}

	/**
	* \brief Retrieves attached property for parsed path.
	*
	* \details Concrete binary layout of vector depends from type of property.
	*/
	//std::vector<unsigned char> getProperty(const BoardPeripheral &peripheral, const KeyPath &keyPath);

	/**
	* \brief Reads attached property for parsed path.
	*/
	//EmptyResult readProperty(const BoardPeripheral &peripheral, const KeyPath &keyPath);

	/**
	* \brief Writes attached property for parsed path for given peripheral.
	*
	* \details Concrete binary layout of vector depends from type of property.
	*/
	//EmptyResult writeProperty(const BoardPeripheral &peripheral, const KeyPath &keyPath, const std::vector<unsigned char> value);
}
#endif

#ifdef __cplusplus  
extern "C" {
#endif  

	/** \brief Board peripheral stream arriving. */
	typedef void(GSDK_CALL* GSdkBoardStreamEvent)(int streamType, int length, void* stream, void* userInfo);

	GSDK_API int GSDK_CALL GSdkBoardPeripheralConnectStreamCallback(GSdkOwnedPointer handle, GSdkBoardStreamEvent callback, void* userInfo);

	GSDK_API void GSDK_CALL GSdkBoardPeripheralDisconnectStreamCallback(GSdkOwnedPointer handle, int identifier);

	GSDK_API void GSDK_CALL GSdkBoardPeripheralDisconnectAllStreamCallbacks(GSdkOwnedPointer handle);

	GSDK_API GSdkResult GSDK_CALL GSdkBoardPeripheralReboot(GSdkOwnedPointer handle);

	GSDK_API GSdkResult GSDK_CALL GSdkBoardPeripheralTare(GSdkOwnedPointer handle);

	GSDK_API GSdkResult GSDK_CALL GSdkBoardPeripheralResetImuFilter(GSdkOwnedPointer handle);

	GSDK_API GSdkResult GSDK_CALL GSdkBoardPeripheralCalibrateGyro(GSdkOwnedPointer handle);

	GSDK_API GSdkResult GSDK_CALL GSdkBoardPeripheralCalibrateBaro(GSdkOwnedPointer handle);

	GSDK_API GSdkResult GSDK_CALL GSdkBoardPeripheralCommit(GSdkOwnedPointer handle);

	GSDK_API GSdkResult GSDK_CALL GSdkBoardPeripheralBootToDFU(GSdkOwnedPointer handle);

	GSDK_API GSdkResult GSDK_CALL GSdkBoardPeripheralResetSettings(GSdkOwnedPointer handle);

	GSDK_API GSdkResult GSDK_CALL GSdkBoardPeripheralSetConnectionInterval(GSdkOwnedPointer handle, float value);

#ifdef __cplusplus  
}
#endif 
