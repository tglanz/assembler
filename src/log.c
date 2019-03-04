#include "log.h"

unsigned int gLogLevel = LOG_DEFAULT_LEVEL;

#ifdef __unix__ 
    void logSetTerminalColor(LogLevel logLevel){
        switch (logLevel){
            case LOG_LEVEL_ERROR:
                printf("\x1B[31m");
                break;
            case LOG_LEVEL_WARNING:
                printf("\x1B[33m");
                break;
            case LOG_LEVEL_INFO:
                printf("\x1B[32m");
                break;
            case LOG_LEVEL_DEBUG:
            default:
                break;
        }
    }

    void logResetTerminalColor(){
        printf("\x1B[0m");
    }
#else
    void logSetTerminalColor(){
    }

    void logResetTerminalColor(){
    }
#endif

void logSetLevel(LogLevel level){
    gLogLevel = level;
}

void writeHeader(LogLevel logLevel){

    string header; 
    time_t raw;
    struct tm * timeInfo;

    switch (logLevel){
        case LOG_LEVEL_ERROR:
            header = "EROR";
            break;
        case LOG_LEVEL_WARNING:
            header = "WARN";
            break;
        case LOG_LEVEL_INFO:
            header = "INFO";
            break;
        case LOG_LEVEL_DEBUG:
            header = "DEBG";
            break;
        default:
            header = "UNKN";
            break;
    }

    time(&raw);
    timeInfo = localtime(&raw);

    printf("%02d:%02d:%02d ",
        timeInfo->tm_hour,
        timeInfo->tm_min,
        timeInfo->tm_sec);

    logSetTerminalColor(logLevel);
    printf("[%s] ", header);
    logResetTerminalColor();
}

void logDebug(string fmt, ...) {
    va_list args;
    if (LOG_LEVEL_DEBUG >= gLogLevel){
        writeHeader(LOG_LEVEL_DEBUG);
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}

void logInfo(string fmt, ...) {
    va_list args;
    if (LOG_LEVEL_INFO >= gLogLevel){
        writeHeader(LOG_LEVEL_INFO);
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}

void logWarning(string fmt, ...) {
    va_list args;
    if (LOG_LEVEL_WARNING >= gLogLevel){
        writeHeader(LOG_LEVEL_WARNING);
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}

void logError(string fmt, ...) {
    va_list args;
    if (LOG_LEVEL_ERROR >= gLogLevel){
        writeHeader(LOG_LEVEL_ERROR);
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }
}
