/*
 ============================================================================
 Name        : Logger.h
 Author      : Andrea Graziani
 Version     : 1.0
 Description : ANSI C implementation of a logger. (HEADER)
 ============================================================================
*/

#ifndef TEN_PROJECT_LOGGER_H
#define TEN_PROJECT_LOGGER_H

#include <stdio.h>

const char *ERROR_LOG_LEVEL = "Error";
const char *INFO_LOG_LEVEL = "Info";
#ifdef DEBUG_MODE
const char* DEBUG_LOG_LEVEL = "Debug";
#endif

typedef struct {
    FILE *logFile;
} Logger;

typedef char *LogLevel;

/**
 *
 * @return
 */
Logger *initLogger();

/**
 *
 * @param logger
 */
void freeLogger(Logger *logger);

/**
 *
 * @param logger
 * @param logLevel
 * @param moduleName
 * @param format
 * @param ...
 */
void logMessage(Logger *logger, LogLevel logLevel, char *moduleName, const char *format, ...);


#endif //TEN_PROJECT_LOGGER_H
