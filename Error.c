#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void exitPrintingFatalErrorMessage(char *functionName, char *errorMessageOrSyscallName) {

    if (errno != 0)
        fprintf(stderr, "[FATAL] '%s': %s --> %s\n", functionName, errorMessageOrSyscallName, strerror(errno));
    else
        fprintf(stderr, "[FATAL] '%s': %s\n", functionName, errorMessageOrSyscallName);

    exit(EXIT_FAILURE);
}