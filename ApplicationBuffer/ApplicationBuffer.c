#include "ApplicationBuffer.h"
#include "../Math/Number.h"

ApplicationBuffer *allocateApplicationBuffer() {

    ApplicationBuffer *output = malloc(sizeof(ApplicationBuffer));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->auxiliaryNumbersBuffer = allocateNumbersArray(AuxiliaryNumbersBufferLength);
    }

    return output;
}

__mpz_struct *getAuxiliaryNumber(ApplicationBuffer *applicationBuffer, int index) {
    return *(applicationBuffer->auxiliaryNumbersBuffer + index);
}

void deallocateApplicationBuffer(ApplicationBuffer *applicationBuffer) {

    deallocateNumbersArray(applicationBuffer->auxiliaryNumbersBuffer, AuxiliaryNumbersBufferLength);
    free(applicationBuffer);
}
