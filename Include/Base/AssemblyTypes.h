#pragma once

#include "Base/Result.h"

#ifdef __cplusplus
#include <string>
#include <memory>
#include <vector>
#endif

#if defined(__GNUC__) || defined(__GNUG__)
#ifndef __cdecl
#define __cdecl __attribute__((__cdecl__))
#endif
#define GSDK_DLL_EXPORT // Nothing
#else
#define GSDK_DLL_EXPORT __declspec(dllexport)
#endif

#ifdef GSDK_EXPORTS
#define GSDK_API GSDK_DLL_EXPORT
#else
#define GSDK_API // __declspec(dllimport)
#endif

#define GSDK_CALL __cdecl

#ifdef __cplusplus
#define GSDK_EXTERN_BEGIN extern "C" {
#define GSDK_EXTERN_END }
#else
#define GSDK_EXTERN_BEGIN
#define GSDK_EXTERN_END
#endif

#ifdef __cplusplus  
extern "C" {
#endif  
	/**
	* \brief The pointer which is valid only in place of call. Don't try
	* to keep this pointer outside of scope call, it behaviour is undefined.
	*/
	typedef void * GSdkScopedPointer;

	/** \brief Opaque pointer to any handle */
	typedef void * GSdkOwnedPointer;

	/**
	* \brief Acquires scoped pointer. From this time it is possible to use
	* acquired pointer outside of it scope.
	*
	* \param pointer The handle to object to retain.
	* \return An retained pointer to scoped one. You are responsible for it
	* releasing via call `GSdkRelease` method.
	*/
	GSDK_API GSdkOwnedPointer GSDK_CALL GSdkRetain(GSdkScopedPointer pointer);

	/**
	* \brief Releases any onwned pointer. Semantic of this operation is
	* single release, without reference count.
	*
	* \param pointer The handle to object to release. In case object was not
	* cretated by C api nothing will happen.
	*/
	GSDK_API void GSDK_CALL GSdkRelease(GSdkOwnedPointer pointer);

	/**
	* \brief Obtains pointer without keeping it for use in shared table.
	*
	* \param pointer The handle to object to retain.
	* \return An bridged pointer to scoped one. The behaviour outside the
	* call visibility is undefined.
	*/
	GSDK_API GSdkOwnedPointer GSDK_CALL GSdkBridge(GSdkScopedPointer pointer);
#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus  

namespace GSdk {
	namespace Base {
		/** Unused macro*/
		template <typename... Args> inline void unused(Args&& ...) {}

		/** \brief Basic declaration for all passed to external api items */
		class Item {
		public:
			virtual int type() const = 0;
            
            virtual ~Item() {}
		};

		typedef std::shared_ptr<Item> ItemPtr;

		/** \brief Converts pointer to item pointer. */
		template<typename T>
		ItemPtr erase(const std::shared_ptr<T>& value) {
			return std::dynamic_pointer_cast<Item>(value);
		};

		/** \brief Converts item pointer to concrete pointer. */
		template<typename T> T* colorize(const GSdkOwnedPointer& pointer) {
			auto value = (Item*)pointer;
			return dynamic_cast<T*>(value);
		};
	}
}

std::string getString(const bool &value);

std::string getString(const short &value);

std::string getString(const int &value);

std::string getString(const float &value);

std::string getString(const double &value);

std::string getString(const std::string &value);

std::string getString(const std::vector<unsigned char> &value);

#endif
