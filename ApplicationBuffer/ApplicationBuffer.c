#include "ApplicationBuffer.h"
#include "../Math/Number.h"

__mpz_struct **allocateAuxiliaryNumbersBuffer() {

    __mpz_struct **output = malloc(AuxiliaryNumbersBufferLength * sizeof(__mpz_struct *));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        for (int i = 0; i < AuxiliaryNumbersBufferLength; i++)
            *(output + i) = allocateNumber();

    return output;
}

ApplicationBuffer *allocateApplicationBuffer() {

    ApplicationBuffer *output = malloc(sizeof(ApplicationBuffer));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->auxiliaryNumbersBuffer = allocateAuxiliaryNumbersBuffer();
    }

    return output;
}

__mpz_struct *getAuxiliaryNumber(ApplicationBuffer *applicationBuffer, int index) {
    return *(applicationBuffer->auxiliaryNumbersBuffer + index);
}

void freeAuxiliaryNumbersBuffer(ApplicationBuffer *applicationBuffer) {

    for (int i = 0; i < AuxiliaryNumbersBufferLength; i++) {
        mpz_clear(*(applicationBuffer->auxiliaryNumbersBuffer + i));
    }
    free(applicationBuffer->auxiliaryNumbersBuffer);
}