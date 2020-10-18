#pragma once

#include "Base/Logger.h"
#include "Peripheral/PeripheralCentral.h"

#ifdef __cplusplus
namespace GSdkCxxTatto {
	using namespace GSdkCxxBase;
	using namespace GSdkCxxPeripheral;

	class TattoFactory {
	private:
		LoggerPtr m_logger;

	public:
		PeripheralCentralPtr makeTattoCentral();

		TattoFactory();
	};
}
#endif

GSDK_EXTERN_BEGIN

/** \brief. Creates new instance of tatto central. Entry point to manipulate with peripherals. */
GSDK_API GSdkOwnedPointer GSDK_CALL GSdkMakeTattoCentral();

GSDK_EXTERN_END
