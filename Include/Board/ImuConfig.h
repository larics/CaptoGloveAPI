#pragma once

#ifdef __cplusplus
#include <string>
#endif
#include <stdint.h>

#ifdef __cplusplus  
extern "C" {
#endif  
	/** \brief Type of imu filter */
	typedef enum {
		/** \brief Madgwick filter */
		GSdkImuFilterTypeMadgwick = 0,

		/** \brief Mahony filter */
		GSdkImuFilterTypeMahony,
	} GSdkImuFilterType;

	/** \brief Scale of IMU accelerometer */
	typedef enum {
		GSdkAccelerometerScale2G = 0,
		GSdkAccelerometerScale4G = 1,
		GSdkAccelerometerScale8G = 2,
		GSdkAccelerometerScale16G = 3,
	} GSdkAccelerometerScale;

	typedef enum {
		GSdkGyroScale250DPS = 0,
		GSdkGyroScale500DPS = 1,
		GSdkGyroScale1000DPS = 2,
		GSdkGyroScale2000DPS = 3,
	} GSdkGyroScale;

	/** \brief IMU configuration (affects quaternion stability) */
#pragma pack (push, 1)
	typedef struct {
		unsigned char filterType;
		float madgwickBeta;
		float mahonyKp;
		float mahonyKi;
		float pressureFilterA;
		float magneticFilterA;
		uint32_t tickTime;
		unsigned char accelerometerScale;
		unsigned char gyroScale;
		unsigned char accelerometerDlpfLevel;
		unsigned char gyroDlpfLevel;
	} GSdkImuConfig;
#pragma pack (pop)

	/** \brief Construct balck color */
	extern GSdkImuConfig const GSdkImuConfigDefault;
#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus  

namespace GSdk {
	namespace Board {
		typedef GSdkImuConfig ImuConfig;
	}
}

/** \brief Equality for imu descriptor */
bool operator == (const GSdkImuConfig& lhs, const GSdkImuConfig& rhs);

/** \brief Description of this color */
std::string getString(const GSdkImuConfig &value);

#endif
