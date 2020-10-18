#pragma once

#include "Base/AssemblyTypes.h"

#ifdef __cplusplus
#include <string>
#include <memory>
#include <vector>
#endif

/** \brief Domain for gatt errors */
extern const int GSdkBLEErrorDomain;

#ifdef __cplusplus
namespace GSdk {
	namespace BLE {
		using namespace Base;

		/** \brief Result code of Gatt operation */
		enum ResultCode {
			ResultCodeSuccess = 0,
			ResultCodeInvalidHandle = 1,
			ResultCodeDeviceNotFound = 2,
			ResultCodeServiceNotFound = 3,
			ResultCodeCharacteristicNotFound = 4,
			ResultCodeCharacteristicNotRead = 5,
			ResultCodeCharacteristicNotWrite = 6,
			ResultCodeCharacteristicNotSubscribed = 7,
			ResultCodeDescriptorNotFound = 8,
			ResultCodeDescriptorNotRead = 9,
			ResultCodeDescriptorNotWrite = 10,
			ResultCodeDiscoveringFailed = 11,
		};

		/** \brief Central templated error maker */
		template<typename Payload>
		Result<Payload> makeResult(
			const ResultCode& status,
			const std::string& description = "",
			const std::shared_ptr<Payload>& payload = std::shared_ptr<Payload>()) {
            return Result<Payload>(status, GSdkBLEErrorDomain, description, payload);
		};

		/** Central void error maker */
		Result<void> makeEmptyResult(const ResultCode& status, const std::string& description = "");

		/** \brief Possible gatt flagg for reading or writing anything */
		enum Flag {
			/** \brief The client does not have specific GATT requirements (default). */
			FlagNone = 0x00000000,

			/** \brief The client requests the data to be transmitted over an encrypted channel. */
			FlagConnectionEncrypted = 0x00000001,

			/** \brief The client requests the data to be transmitted over an authenticated channel. */
			FlagConnectionAuthentificated = 0x00000002,

			/** \brief The descriptor value is to be read from the cache (regardless of whether it is present in the cache or not). */
			FlagValueReadFromDevice = 0x00000004,

			/** \brief The descriptor value is to be read directly from the device. This overwrites the one in the cache if one is already present. */
			FlagValueReadFromCache = 0x00000008,

			/** \brief Signed write. Profile drivers must use with BLUETOOTH_GATT_FLAG_WRITE_WITHOUT_RESPONSE in order to produce signed write without a response. */
			FlagSignedWrite = 0x00000010,

			/** \brief  Write without response */
			FlagWriteWithoutResponse = 0x00000020,

			/** \brief Returns all flags mask */
			FlagReturnAll = 0x00000040,
		};
	}
}
#endif
