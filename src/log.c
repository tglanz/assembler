#include "log.h"

unsigned int gLogLevel = LOG_DEFAULT_LEVEL;

void log_set_level(LogLevel level){
    if (level > LOG_LEVEL_ERROR){
        level = LOG_LEVEL_ERROR;
    }

    gLogLevel = level;
}

void writeHeader(const char * header){

    char timeString[10];
    time_t raw;
    struct tm * timeInfo;

    time(&raw);
    timeInfo = localtime(&raw);

    sprintf(timeString,
        "%d:%d:%d",
        timeInfo->tm_hour,
        timeInfo->tm_min,
        timeInfo->tm_sec);

    printf("%s [%s] ", timeString, header);
}

void logDebug(const char * fmt, ...) {
    va_list args;
    if (LOG_LEVEL_DEBUG >= gLogLevel){
        writeHeader("DEBG");
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}

void logInfo(const char * fmt, ...) {
    va_list args;
    if (LOG_LEVEL_INFO >= gLogLevel){
        writeHeader("INFO");
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}

void logWarning(const char * fmt, ...) {
    va_list args;
    if (LOG_LEVEL_WARNING >= gLogLevel){
        writeHeader("WARN");
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}

void logError(const char * fmt, ...) {
    va_list args;
    if (LOG_LEVEL_ERROR >= gLogLevel){
        writeHeader("EROR");
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}

void logFatal(const char * fmt, ...) {
    va_list args;
    writeHeader("FATL");
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    exit(EXIT_CODE_FATAL);
}