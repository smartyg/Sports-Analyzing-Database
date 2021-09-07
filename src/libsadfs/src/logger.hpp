#ifndef _X_LOGGER_HPP_
#define _X_LOGGER_HPP_

#include <cstdint>
#include <exception>
#include <stdio.h>

#include "exceptions.hpp"

#define CRITICAL(format, ...) (Logger::Log(Logger::CRIT, __FILE__, __func__, __LINE__, format, __VA_ARGS__))
#define ERROR(format, ...) (Logger::Log(Logger::ERR, __FILE__, __func__, __LINE__, format, __VA_ARGS__))
#define WARNING(format, ...) (Logger::Log(Logger::WARNING, __FILE__, __func__, __LINE__, format, __VA_ARGS__))
#define NOTICE(format, ...) (Logger::Log(Logger::NOTICE, __FILE__, __func__, __LINE__, format, __VA_ARGS__))
#define INFO(format, ...) (Logger::Log(Logger::INFO, __FILE__, __func__, __LINE__, format, __VA_ARGS__))

#ifdef _RELEASE
#define DEBUG(format, ...) (empty())
inline const void empty (void) { return; };
#else
#define DEBUG(format, ...) (Logger::Log(Logger::DEBUG, __FILE__, __func__, __LINE__, format, __VA_ARGS__))
#endif

class Logger {
public:
	typedef enum : uint_fast8_t {
		EMERG   = 0, // System is unusable
		ALERT   = 1, // Action must be taken immediately
		CRIT    = 2, // Critical conditions
		ERR     = 3, // Error conditions
		WARNING = 4, // Warning conditions
		NOTICE  = 5, // Normal but significant condition
		INFO    = 6, // Informational
		DEBUG   = 7  // Debug-level messages
	} Loglevel;

	static void Log (const Loglevel, const char *, const char *, const int, const char *, ...);
	static void Exception (const Loglevel, const SadExceptions);

	static FILE *getStream (void);
	static Loglevel getLoglevel (void);

	static bool setStream (FILE *);
	static bool setLoglevel (Loglevel);

private:
	static FILE *stream;
	static Loglevel level;

	static const char *levelToString (const Loglevel);
};

#endif /* _X_LOGGER_HPP_ */
