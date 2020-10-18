#pragma once

#ifdef __cplusplus
#include <string>

namespace GSdk {
	namespace Base {
		/** \brief Shared information regarding this library. */
		class AssemblyInfo {
		private:
			AssemblyInfo();
		public:
			/**
			* \brief Provides current assembly version.
			* \details Version is written in format: major.minor.build.
			*/
			static std::string version();

			/**
			* \brief Current compiled version of platform of library
			*/
			static std::string platform();

			/**
			* \brief Provides current assembly release date.
			* Date is written as string in format YYYY-MM-dd.
			*/
			static std::string releaseDate();

			/**
			* \brief Format date and version in user readable format.
			*/
			static std::string description();

			~AssemblyInfo();
		};
	}
}

#endif
