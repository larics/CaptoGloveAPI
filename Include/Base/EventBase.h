#pragma once

#ifdef __cplusplus
#include <functional>

namespace GSdkCxxBase {
	/**
	* \brief Class for observing various event of board. Concrete
	* observable property is in subclasses.
	*/
	template <typename Args>
	class EventBase {
	public:
		/** \brief Disconnects a previously connected function. */
		virtual void disconnect(const int &id) = 0;

		/**  \brief Disconnects all previously connected functions. */
		virtual void disconnect_all() = 0;

		/**
		* Connects a std::function to the signal. The returned
		* value can be used to disconnect the function again
		*/
		virtual int connect(std::function<void(const Args &)> const& slot) = 0;
	};
}

#endif
