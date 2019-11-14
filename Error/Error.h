//
// Created by Andrea on 14/11/2019.
//

#ifndef TEN_PROJECT_ERROR_H
#define TEN_PROJECT_ERROR_H

#include <stddef.h>

typedef struct {
    int errorCode;
    const char *message;
} Error;

const Error ERROR_UNEXPECTED = {1, "Unexpected error occurred."};
const Error ERROR_INVALID_PARAMETER = {2, "Invalid parameter."};

void printErrorAndExit(Error error);

#endif //TEN_PROJECT_ERROR_H
