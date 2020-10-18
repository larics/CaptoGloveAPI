#pragma once

#ifdef __cplusplus
#include <string>
#endif

#ifdef __cplusplus  
extern "C" {
#endif  

	/** \brief Maximum available timeslots count */
#define GSDK_MAX_STREAM_TIMESLOTS 20

	/**
	* \brief Stream timeslots structure for setting amout of timeslots for subscribed data.
	*
	* \details More timeslots for given endpoint leads to more frequent calls of stream callback.
	* Maximum amount of timeslots for all endpoints should not exceed GSDK_MAX_STREAM_TIMESLOTS.
	*/
#pragma pack (push, 1)
	typedef struct {
		/**
		* \brief Mouse emulation timeslots.
		*
		* \details In case of activation the endpoint corresponding events will be sent trough HID.
		* only. No events will be sent trough stream callback.
		*/
		unsigned char mouse;

		/**
		* \brief Keyboard emulation timeslots.
		*
		* \details In case of activation the endpoint corresponding events will be sent trough HID
		* only. No events will be sent trough stream callback.
		*/
		unsigned char keyboard;

		/**
		* \brief Joystick emulation timeslots.
		*
		* \details In case of activation the endpoint corresponding events will be sent trough HID
		* only. No events will be sent trough stream callback.
		*/
		unsigned char joystick;

		/**
		* \brief Tared quaternion data timeslots.
		*
		* \details This data represents tared relative some reference position (which could be set
		* by either short board button click or command tareBoard) quaternion. The quaternion
		* represents rotation matrix for board and can be converted using usual math formulas.
		*/
		unsigned char taredQuaternion;

		/**
		* \brief Absolute altitude data timeslots.
		*
		* \details This data represents absolute altitude of board in meters.
		*/
		unsigned char absoluteAltitude;

		/**
		* \brief Linear acceleration data timeslots.
		*
		* \details This data represents linear acceleration of board, g/s, excluding gravity
		* component.
		*/
		unsigned char linearAcceleration;

		/**
		* \brief Sensors state data timeslots.
		*
		* \details This data represents conductivity from set sensors. Every value of conductivity
		* can be interpreted later, based on type of sensor. See also ExternalSensorAssembly class
		* for default sensors layout as well as valueInverted() of ExternalSensor method.
		*/
		unsigned char sensorsState;

		/**
		* \brief Raw acceleration data timeslots.
		*
		* \details This data represents linear acceleration of board, g/s, without excluding gravity
		* component. Typically used only for debug reasons.
		*/
		unsigned char rawAcceleration;

		/**
		* \brief Calibrated magnetism data.
		*
		* \details This data represents received and calibrated data drom board magnetormeter. It is
		* written as 3 component float vector with magnetic influence for 3 cartesian space axis.
		*/
		unsigned char calibratedMagnetism;

		/**
		* \brief Raw gyro data.
		*
		* \details This data represents raw board gyroscope rotation rates. Typically used for debug
		* reasons.
		*/
		unsigned char gyroRate;

		/**
		* \brief Relative altitude data timeslots.
		*
		* \details This data represents relative altitude of board in meters about reference position
		* (which could be set by either short board button click or command tareBoard).
		*/
		unsigned char taredAltitude;

		/**
		* \brief Absolute air pressure data.
		*
		* \details Represents air pressure to board, in giga pascals.
		*/
		unsigned char pressure;

		/**
		* \brief Raw magnetism data.
		*
		* \details This data represents received and non calibrated data drom board magnetormeter. It
		* is written as 3 component float vector with magnetic influence for 3 cartesian space axis.
		* Typically used only for debug reasons.
		*/
		unsigned char rawMagnetism;
	} GSdkStreamTimeslots;
#pragma pack (pop)
	
	/** 
	* @brief Are two stream timeslots equal
	*/
	bool GSdkStreamTimeslotsEqual(GSdkStreamTimeslots lhs, GSdkStreamTimeslots rhs);
#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus 

namespace GSdk {
	namespace Board {
		typedef GSdkStreamTimeslots StreamTimeslots;

		/** \brief Provides empty stream timeslots */
		StreamTimeslots getEmptyStreamTimeslots();

		/** \brief Returns total count of used timeslots */
		int getStreamTimeslotsCount(const StreamTimeslots &timeslots);

		/** \brief Is this timeslots valid */
		bool isStreamTimeslotsValid(const StreamTimeslots& timeslots);

		/** \brief Equality operator for 2 stream timeslots */
		bool operator ==(const StreamTimeslots& lhs, const StreamTimeslots& rhs);
	}
}

/** \brief Description of this timeslots */
std::string getString(const GSdkStreamTimeslots &value);

#endif
