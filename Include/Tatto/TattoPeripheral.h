#pragma once

#include "Base/AssemblyTypes.h"
#include "Base/Event.h"
#include "Peripheral/PeripheralBase.h"
#include "Tatto/TattoTypes.h"

#ifdef __cplusplus
#include <string>
#include <memory>

namespace GSdk {
	namespace Tatto {
		using namespace Peripheral;

		/**
		* \brief Major class for interaction with tatto haptic bracelet.
		*/
		class TattoPeripheral : public virtual PeripheralBase {
		public:
			/**
			* \brief Current temperature of this bracelet. Supports the following values:
			* 0.5 - neutral, values between 0.0 and 0.5 - cold, where 0.0 very cold, values
			* between 0.5 and 1.0 - hot, where 1.0 - very hot.
			*/
			virtual float temperature() const = 0;

			/** \brief Bracelet temperature getter */
			virtual EmptyResult temperatureRead() = 0;

			/** \brief Bracelet temperature setter */
			virtual EmptyResult temperatureWrite(const float& value) = 0;

			/**
			* \brief Retrieves attached property for parsed path.
			*
			* \details Concrete binary layout of vector depends from type of property.
			* Subclasses should override this method.
			*/
			std::vector<unsigned char> property(const KeyPath& keyPath) const;

			/**
			* \brief Reads attached property for parsed path.
			*
			* \details Subclasses should override this method.
			*/
			EmptyResult readProperty(const KeyPath& keyPath);

			/**
			* \brief Writes attached property for parsed path.
			*
			* \details Subclasses should override this method.
			*/
			EmptyResult writeProperty(const KeyPath& keyPath, const std::vector<unsigned char> prop);
		};

		typedef std::shared_ptr<TattoPeripheral> TattoPeripheralPtr;
	}
}

#endif
