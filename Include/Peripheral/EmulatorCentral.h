#pragma once

#include "BLE/Adapter.h"
#include "Peripheral/PeripheralCentral.h"
#include "Peripheral/PeripheralBase.h"

#ifdef __cplusplus  
namespace GSdk {
	namespace Board {
		using namespace Peripheral;

		/**
		* @brief Providing peripheral for given emulator request.
		*/
		class EmulatorPeripheralSource {
		public:
			/**
			* \brief Tryies to make emulating peripheral.
			*/
			virtual PeripheralBasePtr makePeripheral() const = 0;
            
            virtual ~EmulatorPeripheralSource() {}
		};

		/** @brief Shared pointer to EmulatorPeripheralSource */
		typedef std::shared_ptr<EmulatorPeripheralSource> EmulatorPeripheralSourcePtr;

		/**
		* @brief Implementation of PeripheralCentral to emulate peripherals.
		*/
		class EmulatorCentral : public PeripheralCentral {};

		typedef std::shared_ptr<EmulatorCentral> EmulatorCentralPtr;

		/**
		* @brief Constructs given central with sources and count parameters.
		* @param source The central source who can construct peripheral
		* based on passinf parameters.
		* @param discoverCount Count of peripherals who should be discovered.
		*/
		EmulatorCentralPtr makeEmulatorCentral(const EmulatorPeripheralSourcePtr& source, const int& discoverCount = 1);
	}
}
#endif
