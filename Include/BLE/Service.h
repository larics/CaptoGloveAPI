#pragma once

#include "BLE/Characteristic.h"
#include "BLE/Guid.h"

#ifdef __cplusplus
namespace GSdk {
	namespace BLE {
		using namespace Base;

		/**
		* \brief Service represents a peripheral`s service - a collection of data
		* and associated behaviors for accomplishing a function or feature of a
		* device (or portions of that device).
		*/
		class Service {
		protected:
			Service() {};

		public:
			/** 16 bytes unique identifier of this service */
			virtual Guid guid() const = 0;

			/**
			* \brief Discovers this receiver characteristics. After discovering process
			* they can be found in characteristics() method
			* \returns The status of discovering
			*/
			virtual EmptyResult discoverCharacteristics() = 0;

			/**
			* \brief List of previously discovered characterisitics.
			* For discovering please use discoverCharacteristics() methods
			*/
			virtual const std::vector<CharacteristicPtr>& characteristics() const = 0;

			/**
			* \brief Returns characteristic for specific guid.
			* Returns null pointer in case of missed or ommited call to discoverCharacteristics().
			*/
			CharacteristicPtr characteristic(const Guid& guid) const;
            
            virtual ~Service() {}
		};

		/** \brief Pointer to this service */
		typedef std::shared_ptr<Service> ServicePtr;
	}
}
#endif
