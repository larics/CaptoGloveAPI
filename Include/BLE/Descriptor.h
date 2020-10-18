#pragma once

#include "BLE/Types.h"
#include "BLE/DescriptorValue.h"

#ifdef __cplusplus
namespace GSdk {
	namespace BLE {
		using namespace Base;

		/** 
		* @brief Characteristic descriptor
		*/
		class Descriptor {
		protected:
			Descriptor();

		public:
			/** \brief Provides descriptor type */
			virtual DescriptorType descriptorType() const = 0;

			/** \brief Reads value for this descriptor */
			virtual EmptyResult readValue(const Flag& flags = FlagNone) = 0;

			/** \brief Writes value for this descriptor */
			virtual EmptyResult writeValue(const std::shared_ptr<DescriptorValue>& value, const Flag& flags = FlagNone) = 0;

			/** \brief This descriptor value */
			virtual const std::shared_ptr<DescriptorValue>& value() const = 0;

			virtual ~Descriptor();
		};

		typedef std::shared_ptr<Descriptor> DescriptorPtr;
	}
}
#endif
