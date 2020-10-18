#pragma once

#include "BLE/Types.h"
#include "BLE/Guid.h"
#include "BLE/Service.h"
#include "Base/Event.h"

#ifdef __cplusplus
namespace GSdk {
	namespace BLE {
		using namespace Base;

		/** \brief State of peripheral connection */
		enum class DeviceState {
			/** \brief Peripheral completely disconnected */
			disconnected = 0,

			/** \brief Peripheral is successfully connected */
			connected = 1,

			/** \brief User requested connection but peripheral is not connected */
			pending = 2,
		};

		/** \brief Change of the pripheral property */
		struct DevicePropertyName final {
		public:
			/** \brief Status.  */
			static const std::string status;
		};

		/** \brief Event on peripheral property changes */
		typedef Event<std::string> DevicePropertyEvent;

		/**
		* \brief Class for interacting with bluetooth low-energy peripheral.
		*  Allows to discover services and maintain them.
		*/
		class Device {
		protected:
			/** Default constructor. Unavailable. */
			Device() { };

		public:
			/** \brief Unique identifier for this peripheral */
			virtual std::string id() const = 0;

			/** \brief 16 bytes unique identifier of this peripheral class */
			virtual Guid guid() const = 0;

			/** \brief Display name */
			virtual std::string name() const = 0;

			/** \brief Connection status */
			virtual DeviceState status() const = 0;

			/** \brief Is status pararameters like services can be received without connection. */
			virtual bool connectionless() const = 0;

			/**
			* \brief Connects to this peripheral.
			*
			* \param reconnecting Should this peripheral reconnect on failure. In this case
			* user will receive notification on status change on reconnection succeded.
			* @returns The connection result status
			*/
			virtual EmptyResult connect(const bool& reconnecting) = 0;

			/**
			* \brief Disconnects from this peripheral.
			* @returns The disconnection result status
			*/
			virtual void disconnect() = 0;

			/**
			* \brief Discovers services. While this process asks for system to
			* find all belonging to this peripheral services.
			* @returns The discovering result status
			*/
			virtual EmptyResult discoverServices() = 0;

			/** \brief Invokes this signal on any property change */
			virtual DevicePropertyEvent& propertyChanged() = 0;

			/** \brief List of all discovered serivices */
			virtual std::vector<std::shared_ptr<Service>> services() const = 0;

			/** \brief Description of list of all discovered serivices */
			std::string servicesString() const;

			/**
			* \brief Discovers services only if it was not done before.
			* @returns The discovering result status
			*/
			EmptyResult discoverServicesIfNeeded();

			/** \brief Returns service for guid. */
			std::shared_ptr<Service> service(const Guid& guid) const;

            virtual ~Device() {}
		};

		/** \brief Shared pointer to this peripheral. */
		typedef std::shared_ptr<Device> DevicePtr;
	}
}
#endif
