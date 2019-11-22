#include "ApplicationBuffer.h"
#include "../Math/Number.h"

ApplicationBuffer *allocateApplicationBuffer() {

    ApplicationBuffer *output = malloc(sizeof(ApplicationBuffer));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->auxiliaryNumbersBuffer = allocateNumbersArray(AuxiliaryNumbersBufferLength, true);
        output->currentAvailableNumber = 0;
    }

    return output;
}

__mpz_struct *retrieveAuxiliaryNumber(ApplicationBuffer *applicationBuffer) {

    __mpz_struct *output = *(applicationBuffer->auxiliaryNumbersBuffer + applicationBuffer->currentAvailableNumber);
    applicationBuffer->currentAvailableNumber++;

    return output;
}

void releaseAuxiliaryNumber(ApplicationBuffer *applicationBuffer, unsigned long long amount) {
    applicationBuffer->currentAvailableNumber = applicationBuffer->currentAvailableNumber - amount;
}

void deallocateApplicationBuffer(ApplicationBuffer *applicationBuffer) {

    deallocateNumbersArray(applicationBuffer->auxiliaryNumbersBuffer, AuxiliaryNumbersBufferLength);
    free(applicationBuffer);
}
