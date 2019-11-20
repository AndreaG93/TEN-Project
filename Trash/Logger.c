/*
 ============================================================================
 Name        : Trash.c
 Author      : Andrea Graziani
 Version     : 1.0
 Description : ANSI C implementation of a logger.
 ============================================================================
*/

#include "Logger.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

Logger *initLogger(const char *pathname) {

    Logger *output = malloc(sizeof(Logger));
    if (output == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    output->logFile = fopen(pathname, "a");
    if (output->logFile == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    return output;
}

void freeLogger(Logger *logger) {

    if (fclose(logger->logFile) == 0) {
        free(logger);
    } else {
        exit(EXIT_FAILURE);
    }
}

void logMessage(Logger *logger, LogLevel logLevel, char *moduleName, const char *format, ...) {

    va_list arg;
    struct timeval now;

    gettimeofday(&now, 0);
    fprintf(logger->logFile, "[%s] [%ld.%ld] :%s: ", logLevel, now.tv_sec, now.tv_usec, moduleName);

    va_start(arg, format);
    vfprintf(logger->logFile, format, arg);
    va_end(arg);
}