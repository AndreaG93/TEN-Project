#pragma once

#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>

#define GMPBufferLength 10

__mpz_struct *allocateAndInitializeNewGMPNumber() {
    __mpz_struct *output = malloc(sizeof(__mpz_struct));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        mpz_init(output);
        return output;
    }
}

__mpz_struct *getNumberFromApplicationBuffer(__mpz_struct **buffer, int index) {
    return *(buffer + index);
}

__mpz_struct **allocateAuxiliaryBuffer() {

    __mpz_struct **output = malloc(GMPBufferLength * sizeof(__mpz_struct *));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        for (int i = 0; i < GMPBufferLength; i++)
            *(output + i) = allocateAndInitializeNewGMPNumber();

    return output;
}

void printNumber(__mpz_struct *input, const char *messageLabel) {
    fprintf(stderr, "%s: %s\n", messageLabel, mpz_get_str(NULL, 10, input));
}

void freeAuxiliaryBuffer(__mpz_struct **appBuffer) {

    for (int i = 0; i < GMPBufferLength; i++) {
        mpz_clear(*(appBuffer + i));
    }
    free(appBuffer);
}