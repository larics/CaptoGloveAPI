#pragma once

#include "Peripheral/EmulatorCentral.h"

#ifdef __cplusplus
namespace GSdk {
	namespace Board {
		using namespace Base;
		using namespace Peripheral;

		/**
		* @brief Providing emulating peripheral.
		*/
		class BoardEmulatorPeripheralSource : public EmulatorPeripheralSource {
		private:
			mutable int m_index = 0;

		public:
			PeripheralBasePtr makePeripheral() const;
		};

		/** @brief Constructs emulator peripheral central */
		PeripheralCentralPtr makeBoardEmulatorCentral();
	}
}
#endif

GSDK_EXTERN_BEGIN

/** \brief. Creates new instance of emulating board central.  */
GSDK_API GSdkOwnedPointer GSDK_CALL GSdkMakeBoardEmulatorCentral();

GSDK_EXTERN_END
