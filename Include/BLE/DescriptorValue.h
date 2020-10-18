#pragma once

#ifdef __cplusplus
namespace GSdk {
	namespace BLE {
		enum DescriptorType {
			DescriptorTypeCharacteristicExtendedProperties,
			DescriptorTypeCharacteristicUserDescription,
			DescriptorTypeClientCharacteristicConfiguration,
			DescriptorTypeServerCharacteristicConfiguration,
			DescriptorTypeCharacteristicFormat,
			DescriptorTypeCharacteristicAggregateFormat,
			DescriptorTypeCustomDescriptor
		};

		/**
		 * @brief Descriptor value for characteristic.
		 */
		class DescriptorValue {
		private:
			DescriptorType _descriptorType;

		protected:
			DescriptorValue(const DescriptorType& descriptorType);

		public:
			~DescriptorValue();

			DescriptorType descriptorType() const;
		};

		class GattClientCharacteristicDescriptorValue : public DescriptorValue {
		public:
			GattClientCharacteristicDescriptorValue();

			bool subscribeToNotification;
			bool subscribeToIndication;
		};
	}
}
#endif
