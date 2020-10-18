#pragma once

#include "Base/Logger.h"
#include "Base/Event.h"
#include "BLE/Types.h"
#include "BLE/Guid.h"
#include "BLE/Device.h"

#ifdef __cplusplus
namespace GSdk {
	namespace BLE {
		using namespace Base;

		/** \brief Predicate for enumarating peripherals. */
		typedef std::function<bool(const DevicePtr&)> DevicePredicate;

		/** \brief Options for scan. */
		struct AdapterScanOptions {
		private:
			double m_interval;
		public:
			/** \brief Preferred scan interval, in seconds */
			double interval() const;

			AdapterScanOptions(const double& interval) :
				m_interval(interval) {
				// Do nothing
			}
		};

		/** @brief Event args, sending when adapter changes it scan status. */
		class AdapterScanEventArgs {
		private:
			bool m_scan;
			bool m_status;
			std::string m_reason;

		public:
			/** \brief scan mode */
			const bool& scan() const { return m_scan; }

			/** \brief scan result */
			const bool& status() const { return m_status; }

			/** \brief scan failure reason */
			const std::string& reason() const { return m_reason; }

			/** \brief Constructs this update with result */
			AdapterScanEventArgs(const bool& scan, const bool& status, const std::string& reason) :
				m_scan(scan), m_status(status), m_reason(reason) {
				// Do nothing
			}
		};

		/** @brief Event args sending when adapter finds new devices or looses old ones. */
		class AdapterDevicesEventArgs {
		private:
			std::vector<DevicePtr> m_inserted;
			std::vector<DevicePtr> m_removed;

		public:
			/** \brief Inserted items */
			const std::vector<DevicePtr>& inserted() const { return m_inserted; }

			/** \brief Removed items */
			const std::vector<DevicePtr>& removed() const { return m_removed; }

			/** \brief Constructs this update with peripherals */
			AdapterDevicesEventArgs(
				const std::vector<DevicePtr>& inserted,
				const std::vector<DevicePtr>& removed) :
				m_inserted(inserted), m_removed(removed) {
				// Do nothing
			}

			AdapterDevicesEventArgs() {
				// Do nothing
			}
		};

		typedef Event<AdapterScanEventArgs> AdapterScanEvent;
		typedef Event<AdapterDevicesEventArgs> AdapterDevicesEvent;

		/** \brief Base manager class which is holding peripherals. */
		class Adapter {
		public:
			/**
			* \brief Starts discovering board peripherals. After discovering
			* process user can access discovered peripherals via 'peripherals()'
			*
			* @returns Result of discovering
			*/
			virtual void startScan(const AdapterScanOptions& options) = 0;

			/**
			* \brief Stops discovering board peripherals. Previously discovered
			* peripherals remain valid.
			*
			* @returns Result of discovering
			*/
			virtual void stopScan() = 0;

			/** \brief Invokes this signal on scan events */
			virtual AdapterScanEvent& scanChanged() = 0;

			/** \brief Invokes this signal on peripherals update. */
			virtual AdapterDevicesEvent& devicesChanged() = 0;

			/** \brief Provides list of connected peripherals. */
			virtual std::vector<DevicePtr> devices() const = 0;

			/**
			* \brief Provides list of connected peripherals, filtered by some predicate.
			* \param predicate The filtering predicate.
			* \returns List of filtered and discovered peripherals.
			*/
			std::vector<DevicePtr> devices(const DevicePredicate& predicate) const;
            
            virtual ~Adapter() {}
		};

		typedef std::shared_ptr<Adapter> AdapterPtr;

		/** \brief Initializes this update from old and new list of peripherals */
		AdapterDevicesEventArgs compare(
			const std::vector<DevicePtr>& previous,
			const std::vector<DevicePtr>& current);
	}
}
#endif
