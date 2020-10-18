#pragma once

#include "Base/AssemblyTypes.h"
#include "Peripheral/PeripheralTypes.h"

#ifdef __cplusplus
#include <vector>
#else
#include <stdint.h>
#endif

#ifdef __cplusplus
namespace GSdk {
	namespace Peripheral {
		using namespace Base;

		/** \brief Keypath with indices. Raises exception on spliting or numeric failures. */
		struct KeyPath final {
		public:
			std::string name;
			std::vector<int> indexes;

			KeyPath(const std::string& value);
		};

		/**
		* \brief Major class for being inheritated by all peripherals.
		*/
		class PeripheralBase : public virtual Item {
		public:
			/** \brief Invokes this signal on any property change */
			virtual PeripheralPropertyEvent& propertyChanged() = 0;

			/** \brief 16 bytes unique identifier of this Peripheral. */
			virtual std::string id() const = 0;

			/** \brief Provides name for this board on most available application language. */
			virtual std::string name() const = 0;

			/** \brief Current connection status of board. */
			virtual GSdkPeripheralStatus status() const = 0;

			/**
			* \brief Discovers services and prepares device for work. While this process
			* asks for system to find all belonging to this Peripheral services.
			* @returns result of start operation.
			*/
			virtual EmptyResult start() = 0;

			/**
			* \brief Stops dedicated board. While this process disconnects from board
			* peripheral and stops all corresponding services and characteristics.
			*/
			virtual EmptyResult stop() = 0;

			/**
			* \brief Retrieves attached property for parsed path.
			*
			* \details Concrete binary layout of vector depends from type of property.
			* Subclasses should override this method.
			*/
			virtual std::vector<unsigned char> property(const KeyPath& keyPath) const;

			/**
			* \brief Reads attached property for parsed path.
			*
			* \details Subclasses should override this method.
			*/
			virtual EmptyResult readProperty(const KeyPath& keyPath);

			/**
			* \brief Writes attached property for parsed path.
			*
			* \details Subclasses should override this method.
			*/
			virtual EmptyResult writeProperty(const KeyPath& keyPath, const std::vector<unsigned char> prop);

			/**
			* \brief Retrieves attached property for name. Concrete binary layout of vector
			* depends from type of property.
			*/
			std::vector<unsigned char> property(const std::string& key) const;

			/**
			* \brief Reads attached property for name.
			*/
			EmptyResult readProperty(const std::string& key);

			/**
			* \brief Writes attached property for name.
			*/
			EmptyResult writeProperty(const std::string& key, const std::vector<unsigned char> prop);
		};

		/** \brief Shared pointer to this peripheral. */
		typedef std::shared_ptr<PeripheralBase> PeripheralBasePtr;
	}
}
#endif

GSDK_EXTERN_BEGIN

/** \brief Base peripheral property change. */
typedef void(GSDK_CALL *GSdkPeripheralPropertyEvent)(const char *propertyType, int32_t read, void *userInfo);

GSDK_API GSdkResult GSDK_CALL GSdkPeripheralBaseStart(GSdkOwnedPointer handle);

GSDK_API GSdkResult GSDK_CALL GSdkPeripheralBaseStop(GSdkOwnedPointer handle);

/**
* \brief Universal function to get property of peripheral.
*
* \details Typically scenario of using is to call it twice, one time for defining
* size of output data, one time to get actual data. Passing null value will tend
* to only returning required length of property.
*
* @param handle The opaque pointer to peripheral.
* @param name The property name is from PeripheralProperty
* @param value An output value. Can be passed zero to get value size.
* @param length The length of writing value.
* @return Result of operation
*/
GSDK_API GSdkResult GSDK_CALL GSdkPeripheralBaseGetProperty(GSdkOwnedPointer handle, const char *name, void *value, int *length);

GSDK_API GSdkResult GSDK_CALL GSdkPeripheralBaseReadProperty(GSdkOwnedPointer handle, const char *name);

GSDK_API GSdkResult GSDK_CALL GSdkPeripheralBaseWriteProperty(GSdkOwnedPointer handle, const char *name, const void *pointer, const int length);

GSDK_API int GSDK_CALL GSdkPeripheralBaseConnectPropertyCallback(GSdkOwnedPointer handle, GSdkPeripheralPropertyEvent callback, void *userInfo);

GSDK_API void GSDK_CALL GSdkPeripheralBaseDisconnectPropertyCallback(GSdkOwnedPointer handle, int identifier);

GSDK_EXTERN_END
