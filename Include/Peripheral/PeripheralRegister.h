#pragma once

#include "Base/AssemblyTypes.h"

#ifdef __cplusplus
namespace GSdk {
	namespace Peripheral {
		using namespace Base;

		/** \brief Peripheral register for read write properties */
		template<typename Payload>
		class ReadableRegister {
		public:
			/** \brief Current cached payload */
			virtual const Payload& value() const = 0;

			/** \brief Reads and updates current value. Marked as const for syntax. */
			virtual EmptyResult read() const = 0;
            
            virtual ~ReadableRegister() {}
		};

		/** \brief Peripheral register for read write properties */
		template<typename Payload>
		class WritableRegister : public virtual ReadableRegister<Payload> {
		public:
			/** \brief Writes and updates current value. Marked as const for syntax. */
			virtual EmptyResult write(const Payload& value) const = 0;
		};
	}
}
#endif
