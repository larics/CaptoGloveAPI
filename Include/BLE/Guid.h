#pragma once

#ifdef __cplusplus
#include <string>
#include <vector>
#endif

#ifdef __cplusplus
namespace GSdk {
	namespace BLE {
		/**
		* @brief Unique 16 byte device identifier.
		*/
		struct Guid {
		private:
			std::vector<unsigned char> _bytes;

		public:
			static std::string print(const unsigned short& value);

			/** \brief Underlying 16 bytes array of udid components */
			const std::vector<unsigned char>& bytes() const;

			/** \brief Underlying short address bytes of this udid */
			unsigned short address() const;

			/** \brief Is this guid written in short form */
			bool shortened() const;

			/**
			* \brief Constructs instance of guid with given bytes.
			* \param bytes Sequence of guid bytes, should be 16 bytes length
			*/
			Guid(const unsigned char* bytes);

			/**
			* \brief Constructs instance of guid with short from
			* \param address Short address
			*/
			Guid(const unsigned short& address);

			/**
			* \brief Constructs instance of receiver with given text.
			* \param text Guid text, can written either in short ot long forms.
			*/
			Guid(const std::string& text);

			/** \brief Default constructor */
			Guid();

			/** \brief Destructor */
			~Guid();

			bool operator==(const Guid& other) const;
			bool operator!=(const Guid& other) const;

			/// Byte by byte addition of guids
			Guid operator |(const Guid& other) const;

			/// Byte by byte addition of guids
			void operator |=(const Guid& other);

			/**
			* @brief Bluetooth LE device interface GUID.
			* @warning To provide isolation between services, windows requires that
			* when reading or writing to the device, that the application opens a handle
			* to the service instance itself. This can be accomplished by altering
			* BTDeviceInterfaceGUID in your example to be equal to the service UUID
			* (extended to 128 bit using the standard SIG extension if the service uses
			* a 16 bit UUID) of the service you intended to use.
			*/
			static Guid BluetoothDeviceInterface;

			/** @brief Bluetooth GATT LE device interface GUID */
			static Guid BluetoothGattGattServiceDeviceInterface;

			/** @brief Base UDID for constructing long service from short one */
			static Guid BluetoothBaseUUID;
		};

		/** \brief Hex string of this guid */
		std::string to_string(const Guid& value);
	}
}
#endif