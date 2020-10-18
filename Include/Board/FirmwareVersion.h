#pragma once

#include "Base/AssemblyTypes.h"

#ifdef __cplusplus
#include <string>
#endif

#ifdef __cplusplus  
extern "C" {
#endif  
	/** \brief Description on firmware */
	typedef struct {
		/** \brief Major version of firmware */
		int major;
	} GSdkFirmwareVersion;

	/** 
	* \brief Compares two versions. 
	* \param lhs Left version to compare
	* \param rhs Right version to compare
	* \return -1 if lhs less that right, +1 if right less that left, 0 if versions are equal
	*/
	extern int GSdkFirmwareVersionCompare(GSdkFirmwareVersion lhs, GSdkFirmwareVersion rhs);

	extern GSdkFirmwareVersion GSdkFirmwareVersionMake(int major);
#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus  

namespace GSdk {
	namespace Board {
		typedef GSdkFirmwareVersion FirmwareVersion;
	}
}

/** \brief Description of this version */
std::string getString(const GSdkFirmwareVersion &value);

/** \brief Parses from string */
bool tryParse(const std::string &value, GSdkFirmwareVersion &version);

#endif
