#pragma once

#include "Base/AssemblyTypes.h"
#include "Base/Event.h"

#ifdef __cplusplus
extern "C" {
#endif
	/**
	* \brief Basic domain for this peripheral.
	*
	* \details Some related properties, like error, depends on this domain.
	*/
	extern const int GSdkPeripheralDomain;

	/** \brief Current status of board. */
	typedef enum {
		/** \brief We were unable to read characteristics of device. */
		GSdkPeripheralStatusDisconnected = 0,

		/** \brief We read characteristics of device. */
		GSdkPeripheralStatusConnected = 1,

		/** \brief Peripheral is still connecting. */
		GSdkPeripheralStatusConnecting = 2,
	} GSdkPeripheralStatus;

	/** \brief Possible error codes for result */
	typedef enum {
		GSdkPeripheralErrorInvalidParameter = 1,
		GSdkPeripheralErrorUnsupported = 2,
	} GSdkPeripheralError;
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace GSdk {
	namespace Peripheral {
		using namespace Base;

		/** \brief Change of the pripheral property */
		struct PeripheralProperty final {
		public:
			/** \brief Status. Returns int * as value with size of 4 bytes. */
			static const std::string status;

			/** \brief Name. Returns UTF8 null terminated string as value. */
			static const std::string name;

			/** \brief Identifier. Returns UTF8 null terminated string as value. */
			static const std::string id;
		};

		/** \brief Changed property description */
		struct PeripheralPropertyEventArgs {
			/** \brief Property name */
			std::string name;

			/** \brief Is property already read */
			bool read;

			/** \brief Constructs args with params */
			PeripheralPropertyEventArgs(const std::string& name, const bool& read) : name(name), read(read) {
				// Do nothing
			}
		};

		/** \brief Event on peripheral property changes */
		typedef Event<PeripheralPropertyEventArgs> PeripheralPropertyEvent;

		/** Peripheral void error maker */
		Result<void> makeEmptyResult(const GSdkPeripheralError& status, const std::string& description = "");
	}
}

/** \brief Description of this status */
std::string getString(const GSdkPeripheralStatus &status);

#endif
