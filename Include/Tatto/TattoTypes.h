#pragma once

#include "Base/AssemblyTypes.h"

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
	/** \brief Error domain for tatto errors */
	extern const int GSdkTattoDomain;

	/** \brief Possible error codes for result */
	typedef enum {
		GSdkTatttoErrorDisconnected = 1,
		GSdkTatttoErrorServiceNotFound = 2,
		GSdkTatttoErrorCharacteristicNotFound = 3,
		GSdkTatttoErrorCharacteristicNotRead = 4,
		GSdkTatttoErrorCharacteristicNotWrite = 5,
	} GSdkTatttoError;

#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus
namespace GSdk {
	namespace Tatto {
		using namespace Base;

		/** \brief Change of the pripheral property */
		struct TattoProperty final {
		public:
			/** \brief Temperature. Returns float * as value with size of 4 bytes. */
			static const std::string temperature;
		};

		/** \brief Tatto templated error maker */
		template<typename Payload>
		Result<Payload> makeResult(
			const GSdkTatttoError& status,
			const std::string& description = "",
			const std::shared_ptr<Payload>& payload = std::shared_ptr<Payload>()) {
			return Result<Payload>(status, GSdkTattoDomain, description, payload);
		};

		/** Board void error maker */
		Result<void> makeEmptyResult(const GSdkTatttoError& status, const std::string& description = "");
	}
}

#endif
