#pragma once

#include "Base/Result.h"

#ifdef __cplusplus
namespace GSdkCxxBase {
	template <typename T>
	class Property {
	public:
		virtual const T &value() const = 0;
		virtual EmptyResult read() = 0;
	};

	template <typename T>
	class MutableProperty: public virtual Property<T> {
	public:
		virtual EmptyResult write(const T &value) = 0;
	};
}
#endif
