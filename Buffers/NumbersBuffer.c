#include "NumbersBuffer.h"
#include "../Math/Number.h"

NumbersBuffer *allocateNumbersBuffer(size_t size) {

    NumbersBuffer *output = malloc(sizeof(NumbersBuffer));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->buffer = allocateNumbersArray(size, true);
        output->numbersActuallyReserved = 0;
        output->size = size;
    }

    return output;
}

void freeNumbersBuffer(NumbersBuffer *numbersBuffer) {
    deallocateNumbersArray(numbersBuffer->buffer, numbersBuffer->size);
    free(numbersBuffer);
}

__mpz_struct **retrieveNumbersFromBuffer(NumbersBuffer *numbersBuffer, unsigned char count) {

    __mpz_struct **output = numbersBuffer->buffer + numbersBuffer->numbersActuallyReserved;
    numbersBuffer->numbersActuallyReserved += count;

    return output;
}

__mpz_struct *retrieveNumberFromBuffer(NumbersBuffer *numbersBuffer) {

    __mpz_struct *output = *(numbersBuffer->buffer + numbersBuffer->numbersActuallyReserved);
    numbersBuffer->numbersActuallyReserved++;

    return output;
}

void releaseNumbers(NumbersBuffer *numbersBuffer, unsigned char count) {
    numbersBuffer->numbersActuallyReserved -= count;
}

void releaseNumber(NumbersBuffer *numbersBuffer) {
    numbersBuffer->numbersActuallyReserved--;
}