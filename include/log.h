#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "core.h"

typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_ERROR = 3
} LogLevel;

#define LOG_DEFAULT_LEVEL LOG_LEVEL_INFO

void logSetLevel(LogLevel level);
void logDebug(const char * fmt, ...);
void logInfo(const char * fmt, ...);
void logWarning(const char * fmt, ...);
void logError(const char * fmt, ...);

#endif /* __LOG_H__ */