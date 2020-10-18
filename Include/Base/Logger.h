#pragma once

#include "Base/AssemblyTypes.h"

#ifdef __cplusplus
#include <fstream>
#include <iostream>
#include <cstdarg>
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#endif

#ifdef __cplusplus  
extern "C" {
#endif  
	/** \brief Logging level */
	typedef enum {
		/** \brief Logs only errors. */
		GSdkLogLevelError = 1,

		/** \brief Logs only errors and warns. */
		GSdkLogLevelWarning = 2,

		/** \brief Logs errors, warns, info messages. */
		GSdkLogLevelInfo = 3,

		/** \brief Logs errors, warns, info and debug messages. */
		GSdkLogLevelDebug = 4,

		/** \brief Logs all, including most detailed messages. */
		GSdkLogLevelTrace = 5
	} GSdkLogLevel;

#ifdef __cplusplus  
}
#endif

#ifdef __cplusplus

namespace GSdk {
	namespace Base {
		/** \brief Basic entry for logging messages */
		class LogEntry : public Item {
		public:
			/** \brief Logging level for this logger */
			GSdkLogLevel level;

			int type() const { return 0; }
		public:
			LogEntry() : level(GSdkLogLevelDebug) {}

			/**
			* \brief Logs a message
			* @param message The message to be logged.
			* @param level The level of logging
			*/
			virtual void log(const std::string& message, const GSdkLogLevel& level) = 0;
		};

		/** @brief Shared pointer for LogEntry */
		typedef std::shared_ptr<LogEntry> LogEntryPtr;

		/**
		* @brief Logger compound class which is containing entries.
		*/
		class Logger : public Item {
		public:
			/**
			* Adds new logger entry to loggers array. While this process
			* ownership of entry transfers to stored shared pointer.
			* @param entry: Adding entry
			*/
			virtual void addEntry(LogEntry* entry) = 0;

			/**
			* Adds new logger to loggers array.
			* @param entry: Adding logger
			*/
			virtual void addEntry(const std::shared_ptr<LogEntry>& entry) = 0;

			/**
			* Removes logger entry from loggers array.
			* @param entry: Removing entry
			*/
			virtual void removeEntry(const LogEntry* entry) = 0;

			/**
			* Removes logger entry from loggers array.
			* @param entry: Removing entry
			*/
			virtual void removeEntry(const std::shared_ptr<LogEntry>& entry) = 0;

			/**
			* Removes all loggers
			*/
			virtual void clearEntries() = 0;

			/**
			* Logs a message
			* @param message The message to be logged.
			*/
			virtual void trace(const std::string& message) = 0;

			/**
			* Logs a message
			* @param message The message to be logged.
			*/
			virtual void debug(const std::string& message) = 0;

			/**
			* Logs a message
			* @param message The message to be logged.
			*/
			virtual void info(const std::string& message) = 0;

			/**
			* Logs an error message
			* @param message The message to be logged.
			*/
			virtual void error(const std::string& message) = 0;
		};

		/** @brief Shared pointer to Logger */
		typedef std::shared_ptr<Logger> LoggerPtr;

		/**
		*  @brief Logger entry class to std terminal.
		*/
		class TerminalLogEntry : public LogEntry {
		protected:
			/**
			* Logs a message
			* @param message message to be logged.
			* @param level The level of logging
			*/
			void log(const std::string& message, const GSdkLogLevel& level) override;
		public:
			TerminalLogEntry();
			~TerminalLogEntry();
		};

		/**
		*  @brief Logger entry class to file.
		*/
		class FileLogEntry : public LogEntry {
		private:
			std::ofstream stream;
			void open(const std::string& file);
			void close();
		protected:
			/**
			* Logs a message
			* @param message message to be logged.
			* @param level The level of logging
			*/
			void log(const std::string& message, const GSdkLogLevel& level);
		public:
			FileLogEntry(const std::string& file);
			~FileLogEntry();
		};

		typedef std::function<void(const std::string&, const GSdkLogLevel& level)> CallbackLogFunction;

		/**
		*  @brief Logger entry class to callback.
		*/
		class CallbackLogEntry : public LogEntry {
		private:
			CallbackLogFunction callback;
		protected:
			/**
			* Logs a message
			* @param message message to be logged.
			* @param level The level of logging
			*/
			void log(const std::string& message, const GSdkLogLevel& level);
		public:
			CallbackLogEntry(CallbackLogFunction const& callback);
			~CallbackLogEntry();
		};

		/** \brief Returns shared logger for this SDK. */
		const LoggerPtr& logger();
	}
}

#endif

GSDK_EXTERN_BEGIN

/** \brief Callback for entry */
typedef void(GSDK_CALL *GSdkLogEntryCallback)(const char *message, GSdkLogLevel level, void *userInfo);

/** \brief Get this SDK logger */
GSDK_API GSdkOwnedPointer GSDK_CALL GSdkGetLogger();

/** \brief Adds callback entry to logger. Callback will be called on every message. */
GSDK_API GSdkOwnedPointer GSDK_CALL GSdkMakeCallbackLogEntry(GSdkLogLevel level, GSdkLogEntryCallback callback, void *userInfo);

/** \brief Adds file entry to logger. Entries will be written to this file. */
GSDK_API GSdkOwnedPointer GSDK_CALL GSdkMakeFileLogEntry(GSdkLogLevel level, const char *filename);

/** \brief Adds terminal entry to logger. Entries will be written to screen. */
GSDK_API GSdkOwnedPointer GSDK_CALL GSdkMakeTerminalLogEntry(GSdkLogLevel level);

/** \brief Adds entry to logger.  */
GSDK_API void GSDK_CALL GSdkLoggerAddEntry(GSdkOwnedPointer logger, GSdkOwnedPointer entry);

/** \brief Removes sepcified entry from logger. */
GSDK_API void GSDK_CALL GSdkLoggerRemoveEntry(GSdkOwnedPointer logger, GSdkOwnedPointer entry);

/** \brief Removes all loggers */
GSDK_API void GSDK_CALL GSdkLoggerClearEntries(GSdkOwnedPointer logger);

GSDK_EXTERN_END
