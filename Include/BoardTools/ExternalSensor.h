#pragma once

#ifdef __cplusplus
#include <memory>
#include <string>
#include <vector>

namespace GSdk {
	namespace BoardTools {
		/** \brief Hardware type of sensor */
		enum ExternalSensorType {
			ExternalSensorTypePressure,
			ExternalSensorTypeBending,
		};

		class FingerSensor;

		/** \brief Representing external sensor */
		class ExternalSensor {
		private:
			int m_tag;
			ExternalSensorType m_type;
			bool m_valueInverted;
			std::string m_name;

		public:
			/** \brief Unique identifier of this receiver */
			int tag() const;

			/** \brief Hardware type of this sensor */
			ExternalSensorType type() const;

			/**
			* \brief Returns true in case of value measurement is inverted. This means
			*  that minimum allowed value should be more that maximum one.
			*/
			bool valueInverted() const;

			/** \brief Returns name for this sensor tag */
			std::string name() const;

			ExternalSensor(
				const int& tag,
				const ExternalSensorType& type,
				const bool& valueInverted,
				const std::string& name);

			/** \brief Pressure sensor, mounting on thumb finger */
			static const FingerSensor thumbPressure;

			/** \brief Bending sensor, mounting on thumb finger */
			static const FingerSensor thumbBending;

			/** \brief Pressure sensor, mounting on index finger */
			static const FingerSensor indexPressure;

			/** \brief Bending sensor, mounting on index finger */
			static const FingerSensor indexBending;

			/** \brief Pressure sensor, mounting on middle finger */
			static const FingerSensor middlePressure;

			/** \brief Bending sensor, mounting on middle finger */
			static const FingerSensor middleBending;

			/** \brief Pressure sensor, mounting on ring finger */
			static const FingerSensor ringPressure;

			/** \brief Bending sensor, mounting on ring finger */
			static const FingerSensor ringBending;

			/** \brief Pressure sensor, mounting on pinky finger */
			static const FingerSensor pinkyPressure;

			/** \brief Bending sensor, mounting on pinky finger */
			static const FingerSensor pinkyBending;

			/** \brief Returns registered sensor for given type */
			static const std::vector<int> registeredTags;

			/** \brief Returns registered sensor for given type */
			static const ExternalSensor& registeredSensor(const int& tag);
		};

		typedef std::shared_ptr<ExternalSensor> ExternalSensorPtr;

		/** \brief Finger for sensor */
		enum FingerPlace {
			FingerPlaceThumb,
			FingerPlaceIndex,
			FingerPlaceMiddle,
			FingerPlaceRing,
			FingerPlacePinky,
		};

		/** \brief Finger external sensor */
		class FingerSensor : public ExternalSensor {
		private:
			FingerPlace m_place;

			static std::string getName(const ExternalSensorType& type, const FingerPlace& place);
		public:
			/** \brief Mounting finger for this sensor */
			FingerPlace place() const;

			/** \brief Constructs this sensor with type and finger */
			FingerSensor(const int& tag, const ExternalSensorType& type, const FingerPlace& place);
		};
	}
}

#endif
