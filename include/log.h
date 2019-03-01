/**
 * Logging mechanisms, don't just printf, use different level of logs
 */
#ifndef __LOG_H__
#define __LOG_H__

/* standard */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

/* services */
#include "core.h"

/**
 * Available levels of logs
 */
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_ERROR = 3
} LogLevel;

/* The default log level */
#define LOG_DEFAULT_LEVEL LOG_LEVEL_INFO

/**
 * Globally set the active log level
 * 
 * Arguments
 *      level: LogLevel - the log level to set to
 */
void logSetLevel(LogLevel level);

/**
 * Issues a debug level log entry
 * 
 * Arguments
 *      exactly like printf
 */
void logDebug(string fmt, ...);

/**
 * Issues an info level log entry
 * 
 * Arguments
 *      exactly like printf
 */
void logInfo(string fmt, ...);

/**
 * Issues a warning level log entry
 * 
 * Arguments
 *      exactly like printf
 */
void logWarning(string fmt, ...);

/**
 * Issues an error level log entry
 * 
 * Arguments
 *      exactly like printf
 */
void logError(string fmt, ...);

#endif /* __LOG_H__ */