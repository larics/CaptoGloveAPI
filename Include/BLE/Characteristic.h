#pragma once

#include "BLE/Types.h"
#include "BLE/Descriptor.h"
#include "BLE/Guid.h"

#ifdef __cplusplus
namespace GSdk {
	namespace BLE {
		class Characteristic;

		/// On this characteristic value change
		typedef void(*CharacteristicValueChangedMethod)(const Characteristic& characteristic, const void* context);

		enum ConfigurationDescriptorValue {
			ConfigurationDescriptorValueNone = 0,
			ConfigurationDescriptorValueNotify = 1,
			ConfigurationDescriptorValueIndicate = 2
		};

		/**
		* @brief Gatt service characteristic
		*/
		class Characteristic {
		protected:
			Characteristic();

		public:
			/** @brief Destructor for this characteristic */
			virtual ~Characteristic();

			/** @brief 16 bytes unique identifier of this characteristic */
			virtual Guid guid() const = 0;

			/// Reads descriptors for given characteristic
			virtual EmptyResult readDescriptors(const Flag& flags = FlagNone) = 0;

			/// List of previously discovered descriptors.
			/// For discovering please use readDescriptors() methos
			virtual const std::vector<std::shared_ptr<Descriptor> >& descriptors() const = 0;

			/// Reads value for this characteristic
			virtual EmptyResult readValue(const Flag& flags = FlagNone) = 0;

			/// Writes value for this characteristic
			virtual EmptyResult writeValue(const std::vector<unsigned char>& value, const Flag& flags = FlagNone) = 0;

			/// Current value
			virtual const std::vector<unsigned char>& value() const = 0;

			/// Is this characteristic support broadcast events
			virtual bool broadcastable() const = 0;

			/// Is this characteristic readable
			virtual bool readable() const = 0;

			/// Is this characteristic support notifications
			virtual bool notifiable() const = 0;

			/// Is this characteristic support notifications
			virtual bool indicatable() const = 0;

			/// Is this characteristic writable with response
			virtual bool writableWithResponse() const = 0;

			/// Is this characteristic writable without response
			virtual bool writableWithoutResponse() const = 0;

			virtual std::string description() const = 0;

			/**
			* @brief Sets handler on characteristic value update
			* @param event The pointer to callback function
			* @param context Passing context to this event callback
			*/
			virtual EmptyResult setValueChangedEvent(
				const CharacteristicValueChangedMethod& event,
				const void* context) = 0;

			EmptyResult writeClientCharacteristicConfigurationDescriptor(ConfigurationDescriptorValue value);
		};

		typedef std::shared_ptr<Characteristic> CharacteristicPtr;
	}
}
#endif
