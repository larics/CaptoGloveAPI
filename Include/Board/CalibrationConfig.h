#pragma once

#include "Base/AssemblyTypes.h"
#include "Base/Matrix3.h"

#ifdef __cplusplus
#include <string>
#endif

#ifdef __cplusplus  
extern "C" {
#endif  
#pragma pack (push, 1)
	/** \brief Structure, describing calibration process of different triggers */
	struct GSdkCalibrationConfig {
		/** \brief Matrix for data vector multiplication */
		GSdkMatrix3f matrix;

		/** \brief Constant bias */
		float bias;

		/** \brief Is calibration enabled */
		bool enabled;
	};
#pragma pack (pop)
#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus  

namespace GSdk {
	namespace Board {
		typedef GSdkCalibrationConfig CalibrationConfig;
	}
}

/** \brief Description of this sensor descriptor */
std::string getString(const GSdkCalibrationConfig &value);

/** \brief Is this trigger info equals to another one */
bool operator == (const GSdkCalibrationConfig& lhs, const GSdkCalibrationConfig& rhs);

#endif
