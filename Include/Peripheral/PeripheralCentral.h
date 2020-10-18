#pragma once

#include "Base/Event.h"
#include "Base/AssemblyTypes.h"
#include "Peripheral/PeripheralBase.h"

#ifdef __cplusplus
#include <vector>
#include <memory>
#endif

#ifdef __cplusplus  
extern "C" {
#endif  
	/** \brief Options for scan. */
#pragma pack (push, 1)
	typedef struct {
		double preferredInterval;
	} GSdkScanOptions;
#pragma pack (pop)

#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus

namespace GSdk {
	namespace Peripheral {
		/** @brief Event args, sending when central changes it scan status. */
		class CentralScanEventArgs {
		private:
			bool m_scan;
			bool m_status;
			std::string m_reason;

		public:
			/** \brief scan mode */
			const bool& scan() const { return m_scan; }

			/** \brief scan result */
			const bool& status() const { return m_status; }

			/** \brief scan failure reason description */
			const std::string& reason() const { return m_reason; }

			CentralScanEventArgs(const bool& scan, const bool& status, const std::string& reason) :
				m_scan(scan), m_status(status), m_reason(reason) {
				// Do nothing
			}
		};

		/** @brief Event args sending when central finds new devices or looses old ones. */
		class CentralPeripheralsEventArgs {
		private:
			/** \brief Inserted items */
			std::vector<PeripheralBasePtr> m_inserted;

			/** \brief Removes items */
			std::vector<PeripheralBasePtr> m_removed;

		public:
			/** \brief List of inserted peripherals */
			const std::vector<PeripheralBasePtr>& inserted() const {
				return m_inserted;
			}

			/** \brief List of removed peripherals */
			const std::vector<PeripheralBasePtr>& removed() const {
				return m_removed;
			}

			/** \brief Constructs this receiver with inserted and removed peripherals  */
			CentralPeripheralsEventArgs(
				const std::vector<PeripheralBasePtr>& inserted,
				const std::vector<PeripheralBasePtr>& removed) :
				m_inserted(inserted), m_removed(removed) { }
		};

		typedef Event<CentralScanEventArgs> CentralScanEvent;
		typedef Event<CentralPeripheralsEventArgs> CentralPeripheralsEvent;
		typedef GSdkScanOptions CentralScanOptions;

		/**
		* @brief Major class for scan any supported peripherals. Depending on your 
		* requirements you should select concrete conformant subclass.
		*/
		class PeripheralCentral : public virtual Item {
		public:
			/**
			* \brief Starts discovering board peripherals. After discovering
			* process user can access discovered peripherals via 'peripherals()'
			*
			* @returns Result of discovering
			*/
			virtual void startScan(const CentralScanOptions& options) = 0;

			/**
			* \brief Stops discovering board peripherals. Previously discovered
			* peripherals remain valid.
			*
			* @returns Result of discovering
			*/
			virtual void stopScan() = 0;

			/** \brief Invokes this signal on scan events */
			virtual CentralScanEvent& scanChanged() = 0;

			/** \brief Invokes this signal on peripherals update. */
			virtual CentralPeripheralsEvent& peripheralsChanged() = 0;

			/**
			* \brief List of previously discovered peripherals. Before accessing this
			* list the peripherals should be discovered via `startScan` method.
			*/
			virtual std::vector<PeripheralBasePtr> peripherals() const = 0;
		};

		typedef std::shared_ptr<PeripheralCentral> PeripheralCentralPtr;
	}
}

#endif

GSDK_EXTERN_BEGIN

/** \brief Board scan status change */
typedef void(GSDK_CALL *GSdkPeripheralScanEvent)(int32_t scan, int32_t status, const char *reason, void *userInfo);

/** \brief On board peripherals arriving. */
typedef void(GSDK_CALL *GSdkPeripheralDiscoverEvent)(GSdkScopedPointer *insert, int32_t insertCount, GSdkScopedPointer *remove, int32_t removeCount, void *userInfo);

/** \brief Makes options with values */
GSDK_API GSdkScanOptions GSDK_CALL GSdkScanOptionsMake(double preferredInterval);

GSDK_API void GSDK_CALL GSdkPeripheralCentralStartScan(GSdkOwnedPointer central, GSdkScanOptions options);
GSDK_API void GSDK_CALL GSdkPeripheralCentralStopScan(GSdkOwnedPointer central);
GSDK_API int32_t GSDK_CALL GSdkPeripheralCentralConnectScanChangedEvent(GSdkOwnedPointer central, GSdkPeripheralScanEvent event, void *userInfo);
GSDK_API void GSDK_CALL GSdkPeripheralCentralDisconnectScanChangedEvent(GSdkOwnedPointer central, int32_t identifier);
GSDK_API int32_t GSDK_CALL GSdkPeripheralCentralConnectPeripheralsChangedEvent(GSdkOwnedPointer central, GSdkPeripheralDiscoverEvent event, void *userInfo);
GSDK_API void GSDK_CALL GSdkPeripheralCentralDisconnectPeripheralsChangedEvent(GSdkOwnedPointer central, int32_t identifier);
GSDK_API int32_t GSDK_CALL GSdkPeripheralCentralCountPeripherals(GSdkOwnedPointer handle);
GSDK_API GSdkOwnedPointer GSDK_CALL GSdkPeripheralCentralMakePeripheral(GSdkOwnedPointer handle, int32_t index);

GSDK_EXTERN_END
