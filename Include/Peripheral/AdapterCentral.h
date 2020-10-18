#pragma once

#include "Base/Logger.h"
#include "Base/Event.h"
#include "BLE/Adapter.h"
#include "Peripheral/PeripheralCentral.h"

#ifdef __cplusplus
namespace GSdk {
	using namespace BLE;

	namespace Peripheral {
		/**
		* @brief Providing peripheral for given adapter response.
		*/
		class AdapterPeripheralSource {
		public:
			/**
			* \brief Tries to make peripheral for BLE device.
			*
			* @param device Passing BLE device. Peripheral can
			* be started or no, can have discovered services or no.
			*/
			virtual PeripheralBasePtr makePeripheral(const DevicePtr& device) const = 0;
            
            virtual ~AdapterPeripheralSource() {}
		};

		/** @brief Shared pointer to AdapterPeripheralSource */
		typedef std::shared_ptr<AdapterPeripheralSource> AdapterPeripheralSourcePtr;

		/**
		* @brief Implementation of PeripheralCentral for using it with BLE adapter.
		*/
		class AdapterCentral : public virtual PeripheralCentral {
			/** @brief Can this central resolve adapter while creation. */
			static bool canResolveAdapter();
		};

		/** @brief Shared pointer to AdapterCentral */
		typedef std::shared_ptr<AdapterCentral> AdapterCentralPtr;

		/**
		* @brief Constructs given central with adapter sources
		*
		* @param sources The list of central sources who can construct peripheral
		* based on passing device. Priority of creation is defined by order in list.
		* In case if few sources are able to make peripheral, only first one in list
		* will be used.
		*
		* @param adapter The BLE adapter who will discover devices. In case of passing
		* nullptr central will try to resolve it using default for this compilation unit
		* depending from system type.
		*/
		AdapterCentralPtr makeAdapterCentral(const std::vector<AdapterPeripheralSourcePtr>& sources, const AdapterPtr& adapter = nullptr);
	}
}
#endif
