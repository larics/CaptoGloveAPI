#pragma once

#include "Base/Logger.h"
#include "BLE/Adapter.h"
#include "Peripheral/AdapterCentral.h"

#ifdef __cplusplus
namespace GSdk {
	namespace Tatto {
		using namespace Base;
		using namespace BLE;
		using namespace Peripheral;

		/**
		* @brief Providing peripheral for given adapter response.
		* Peripherals will be of type TattoPeripheral.
		*/
		class TattoAdapterPeripheralSource : public AdapterPeripheralSource {
		private:
			LoggerPtr m_logger;

		public:
            TattoAdapterPeripheralSource();
            
			PeripheralBasePtr makePeripheral(const DevicePtr& device) const;
		};

		/**
		* @brief Constructs peripheral central with adapter. For futher
		* clarification please read documentation for AdapterCentral constructor.
		*/
		PeripheralCentralPtr makeTattoAdapterCentral(const AdapterPtr& adapter = nullptr);
	}
}
#endif

GSDK_EXTERN_BEGIN

/** \brief. Creates new instance of tatto central. Entry point to manipulate with peripherals. */
GSDK_API GSdkOwnedPointer GSDK_CALL GSdkMakeTattoAdapterCentral();

GSDK_EXTERN_END
