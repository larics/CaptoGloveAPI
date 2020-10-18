#pragma once

#include "Base/Logger.h"
#include "Peripheral/PeripheralCentral.h"

#ifdef __cplusplus
namespace GSdkCxxBoard {
	using namespace GSdkCxxBase;
	using namespace GSdkCxxPeripheral;

	class BoardFactory {
	private:
		LoggerPtr m_logger;

	public:
		bool stubMode;
		PeripheralCentralPtr makeBoardCentral();

		BoardFactory();
		~BoardFactory();
	};
}
#endif

GSDK_EXTERN_BEGIN

/** \brief. Creates new instance of board central. Entry point to manipulate with peripherals. */
GSDK_API GSdkOwnedPointer GSDK_CALL GSdkMakeBoardCentral(int stubMode);

GSDK_EXTERN_END
