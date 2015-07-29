#pragma once

#include "mbed.h"
#include <string>

// Ensure that LOG is not defined anywhere else
#ifdef LOG
#undef LOG
#endif


// Do weird macro things for logging the filename and line for every call.
// Also allows for disabling all logging through macros so all log calls can be removed from production builds.
#ifdef RJ_LOGGING_EN

#ifdef __FILE_NAME__
#define __BASE_FILE_NAME__  __FILE_NAME__
#else
#define __BASE_FILE_NAME__  __FILE__
#endif

#define STRINGIFY(x)        #x
#define TO_STRING(x)        STRINGIFY(x)
#define GEN_LOG_STR(x,y)    x ":" TO_STRING(y)
#define LOG_LINE            GEN_LOG_STR(__BASE_FILE_NAME__, __LINE__)
#define LOG(lvl, ...)       log(lvl, LOG_LINE, __func__, __VA_ARGS__)

#else

#define LOG(...)

#endif


#define FOREACH_LEVEL(LEVEL) \
    LEVEL(FATAL)  \
    LEVEL(SEVERE) \
    LEVEL(WARN)   \
    LEVEL(OK)     \
    LEVEL(INF1)   \
    LEVEL(INF2)   \
    LEVEL(INF3)

#define GENERATE_ENUM(ENUM)     ENUM,
#define GENERATE_STRING(STRING) #STRING,


/**
 * Log levels.
 */
enum LOG_LEVEL { FOREACH_LEVEL(GENERATE_ENUM) };


/**
 * Enumeration -> String conversions.
 */
extern const char *LOG_LEVEL_STRING[];


/**
 * Active logging.
 */
extern volatile bool isLogging;


/**
 * Current log level.
 */
extern volatile uint8_t rjLogLevel;


/**
 * [log The system-wide logging interface function. All log messages go through this.]
 * @param logLevel [The "importance level" of the called log message.]
 * @param source   [The source of the message.]
 * @param format   [The string format for displaying the log message.]
 */
extern void log(uint8_t logLevel, const char *source, const char *func, const char *format, ...);
