#pragma once

#ifdef __cplusplus
#include <cstring>
#endif

#include "Board/BoardTypes.h"
#include "Base/Quaternion.h"
#include "Base/Vector3.h"

#ifdef __cplusplus  
extern "C" {
#endif  
	/** \brief Type of streaming structure. */
	typedef enum {
		/** \brief Tared relative reference orientation quaternion. */
		GSdkBoardStreamTypeTaredQuaternion = 3,

		/** \brief Absolue altitude, meters */
		GSdkBoardStreamTypeAbsoluteAltitude = 4,

		/** \brief Linear acceleration, g/s */
		GSdkBoardStreamTypeLinearAcceleration = 5,

		/** \brief Finger sensors states, i.e. an array of sensor conductivities. */
		GSdkBoardStreamTypeSensorsState = 6,

		/** \brief Raw data from accelerometer, without negating gravity component. */
		GSdkBoardStreamTypeRawAcceleration = 7,

		/** \brief Calibrated magnetic field */
		GSdkBoardStreamTypeCalibratedMagnetism = 8,

		/** \brief Raw gyro rate */
		GSdkBoardStreamTypeGyroRate = 9,

		/** \brief Relative altitude from taring point, meters */
		GSdkBoardStreamTypeTaredAltitude = 10,

		/** \brief Atmosphere presuure, gpa */
		GSdkBoardStreamTypePressure = 11,

		/** \brief Raw magnetic measurement. */
		GSdkBoardStreamTypeRawMagnetism = 12,
	} GSdkBoardStreamType;
#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus
namespace GSdk {
	namespace Board {
		/** \brief Event args firing when new data stream is arriving to the board. */
		class BoardStreamEventArgs {
		public:
			/** \brief Type of this stream */
			GSdkBoardStreamType streamType;

			/** \brief Stream raw pointer */
			virtual std::vector<unsigned char> bytes() const = 0;

			/** \brief Constructs given stream args with type */
			BoardStreamEventArgs(const GSdkBoardStreamType& streamType) : streamType(streamType) {}
		};

		/** @brief Arbitrary payload board stream event args. */
		template<typename Payload>
		class BoardStreamPayloadEventArgs : public BoardStreamEventArgs {
		private:
			Payload _value;
		public:
			BoardStreamPayloadEventArgs(
				const GSdkBoardStreamType& streamType,
				const Payload& value) :
				BoardStreamEventArgs(streamType),
				_value(value) {}

			Payload value() const {
				return _value;
			}

			std::vector<unsigned char> bytes() const {
				size_t size = sizeof(_value);
				std::vector<unsigned char> bytes(size);
                std::memcpy(&bytes[0], &_value, size);
				return bytes;
			}
		};

		/** @brief Arbitrary sequence of payload board stream event args. */
		template<typename Payload>
		class BoardStreamSequenceEventArgs : public BoardStreamEventArgs {
		private:
			std::vector<Payload> _value;
		public:
			BoardStreamSequenceEventArgs(
				const GSdkBoardStreamType& streamType,
				const std::vector<Payload>& value) :
				BoardStreamEventArgs(streamType),
				_value(value) {}

			std::vector<Payload> value() const {
				return _value;
			}

			std::vector<unsigned char> bytes() const {
				size_t size = sizeof(_value[0]) * _value.size();
				std::vector<unsigned char> bytes(size);
				std::memcpy(&bytes[0], &_value[0], size);
				return bytes;
			}
		};

		typedef BoardStreamPayloadEventArgs<Quaternionf> BoardQuaternionfEventArgs;
		typedef BoardStreamPayloadEventArgs<Vector3f> BoardVector3fEventArgs;
		typedef BoardStreamPayloadEventArgs<float> BoardFloatValueEventArgs;
		typedef BoardStreamSequenceEventArgs<float> BoardFloatSequenceEventArgs;

		/** \brief On new stream arriving */
		typedef Event<BoardStreamEventArgs> BoardStreamEvent;
	}
}
#endif
