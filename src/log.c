#include "log.h"

unsigned int gLogLevel = LOG_DEFAULT_LEVEL;

void logSetLevel(LogLevel level){
    gLogLevel = level;
}

void writeHeader(string header){

    char timeString[10];
    time_t raw;
    struct tm * timeInfo;

    time(&raw);
    timeInfo = localtime(&raw);

    sprintf(timeString,
        "%02d:%02d:%02d",
        timeInfo->tm_hour,
        timeInfo->tm_min,
        timeInfo->tm_sec);

    printf("%s [%s] ", timeString, header);
}

void logDebug(string fmt, ...) {
    va_list args;
    if (LOG_LEVEL_DEBUG >= gLogLevel){
        writeHeader("DEBG");
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}

void logInfo(string fmt, ...) {
    va_list args;
    if (LOG_LEVEL_INFO >= gLogLevel){
        writeHeader("INFO");
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}

void logWarning(string fmt, ...) {
    va_list args;
    if (LOG_LEVEL_WARNING >= gLogLevel){
        writeHeader("WARN");
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}

void logError(string fmt, ...) {
    va_list args;
    if (LOG_LEVEL_ERROR >= gLogLevel){
        writeHeader("EROR");
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}
