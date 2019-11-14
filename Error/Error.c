//
// Created by Andrea on 14/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include "Error.h"

void printErrorAndExit(const Error error)
{
    fprintf(stderr, "[ERROR] %s\n", error.message);
    exit(error.errorCode);
}
