#pragma once

#ifdef __cplusplus
#include <string>
#include <vector>
#endif

#ifdef __cplusplus  
extern "C" {
#endif

	/** \brief Maximum allowed keys count */
#define GSDK_MAX_TRIGGER_KEYS_COUNT 6

	/**
	* \brief Used target device for trigger.
	* This value defines which logical device key will emulate.
	*/
	typedef enum {
		/** \brief It's mouse device */
		GSdkTargetDeviceMouse = 0,

		/** \brief It's keyboard device */
		GSdkTargetDeviceKeyboard = 1,

		/** \brief It's joystick device */
		GSdkTargetDeviceJoystick = 2,

		/** \brief Special value for emulating specific device control functions */
		GSdkTargetDeviceControl = 3
	} GSdkTargetDevice;

	/**
	* \brief Additional semantic, applyed to key target. For instance, joystick based sources
	* can be splitted for joystick / gamepad, keyboard based for keyboard / steam.
	*/
	typedef enum {
		/** \brief No special info given */
		GSdkKeySemanticUnspecified = 0,

		/** \brief It's gamepad device */
		GSdkKeySemanticGamepad = 0,

		/** \brief It's joystick device */
		GSdkKeySemanticJoystick = 1,

		/** \brief It's keyboard device */
		GSdkKeySemanticKeyboard = 0,

		/** \brief It's Steam VR device */
		GSdkKeySemanticSteam = 1,
	} GSdkKeySemantic;

	typedef enum {
		GSdkKeyTriggerFlagsEmpty = 0x0,
		GSdkKeyTriggerFlagsIgnoresReleaseOnRepeat = 0x01
	} GSdkKeyTriggerFlags;

	/**
	* \brief Trigger, describing firing key for applications.
	*/
	typedef struct {
		/** \brief Is this trigger enabled */
		bool isEnabled;

		/**
		* \brief Emulating target for this key trigger. Key should be
		* sent on behalf of this device.
		*/
		unsigned char target;

		/**
		* Additional semantic, applyed to key target. For instance, joystick based sources
		* can be splitted for joystick / gamepad, keyboard based for keyboard / steam.
		*/
		unsigned char semantic;

		/**
		* \brief Use this option to limit firing keys times during constant
		* pressing of key while glove is in range above. Set zero for disabling
		* limitation, zero to activate function once and so on.
		*/
		unsigned char limit;

		/**
		* \brief Minimum value to raise key, from -Inf to Inf. This value is
		* expressed in units, depending from configuration.
		*/
		float minValue;

		/**
		* \brief Maximum value to raise key, from -Inf to Inf. This value is
		* expressed in units, depending from configuration.
		*/
		float maxValue;

		/** \brief Raising keys */
		short keys[GSDK_MAX_TRIGGER_KEYS_COUNT];

		/**
		* \brief Repeat delay between fires, milliseconds. This property will 
		* take an effect, starting from firmware 181.
		*/
		unsigned int repeatDelay;

		/**
		* \brief Minimum fire action durration, millisecond. This property will 
		* take an effect, starting from firmware 182.
		*/
		unsigned int actionDuration;

		/** 
		* \brief Different additional flags. This property will take an effect, 
		* starting from firmware 182.
		*/
		GSdkKeyTriggerFlags flags;
	} GSdkKeyTrigger;

	/** \brief Determines where or no 2 key triggers are equal */
	bool GSdkKeyTriggerEqualToKeyTrigger(const GSdkKeyTrigger *lhs, const GSdkKeyTrigger *rhs);

	/** \brief Copies one trigger to another */
	void GSdkKeyTriggerCopyTrigger(GSdkKeyTrigger *dest, const GSdkKeyTrigger *source);

#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus  

namespace GSdk {
	namespace Board {
		typedef GSdkKeyTrigger KeyTrigger;

		/** \brief Resets the trigger to initial state */
		void resetKeyTrigger(KeyTrigger& keyTrigger);

		/** \brief Static initial value (after reset) of this trigger */
		KeyTrigger getInitialKeyTrigger();

		/** \brief Returns bytes representation of keys */
		std::vector<unsigned char> getKeyBytes(const KeyTrigger &keyTrigger);

		/**
		* \brief Sets binary bytes to this key set. The size
		* of vector should be less or equalt that MaximumKeys * sizeof(short)
		*/
		void setKeyBytes(KeyTrigger &keyTrigger, const std::vector<unsigned char> keyBytes);

		/** \brief Sets the sole ket to keys */
		void setSoleKey(KeyTrigger& keyTrigger, const short& key);
	}
}

/** \brief Description of this key trigger */
std::string getString(const GSdkKeyTrigger &value);

#endif
