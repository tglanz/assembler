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
void logDebug(string fmt, ...);
void logInfo(string fmt, ...);
void logWarning(string fmt, ...);
void logError(string fmt, ...);

#endif /* __LOG_H__ */