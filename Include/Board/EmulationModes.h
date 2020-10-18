#pragma once

#ifdef __cplusplus
#include <string>
#endif

#ifdef __cplusplus  
extern "C" {
#endif  

	/** \brief Descriptor for enabling HID emulation modes of this board. */
#pragma pack (push, 1)
	typedef struct {
		/** \brief Mouse emulation enabled */
		bool mouse;

		/** \brief Joystick emulation enabled */
		bool joystick;

		/** \brief Fingers emulation enabled */
		bool fingers;

		/** \brief Input axis emulation enabled */
		bool inputAxis;

		/** \brief Acceleration emulation enabled */
		bool acceleration;

		/** \brief Altitude emulation enabled */
		bool barometer;
	} GSdkEmulationModes;
#pragma pack (pop)

#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus  

namespace GSdk {
	namespace Board {
		typedef GSdkEmulationModes EmulationModes;

		/** \brief Provides empty info */
		EmulationModes getEmptyEmulationModes();

		/** \brief Equality operator for 2 emulation modes */
		bool operator ==(const EmulationModes& lhs, const EmulationModes& rhs);
	}
}

/** \brief Description of this modes */
std::string getString(const GSdkEmulationModes &value);

#endif
