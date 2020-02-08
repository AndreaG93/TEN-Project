#pragma once

#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {

    unsigned char size;
    unsigned char numbersActuallyReserved;
    __mpz_struct **buffer;

} NumbersBuffer;

NumbersBuffer *allocateNumbersBuffer(size_t size);

void freeNumbersBuffer(NumbersBuffer *numbersBuffer);

__mpz_struct **retrieveNumbersFromBuffer(NumbersBuffer *numbersBuffer, unsigned char count);

__mpz_struct *retrieveNumberFromBuffer(NumbersBuffer *numbersBuffer);

void releaseNumbers(NumbersBuffer *numbersBuffer, unsigned char count);

void releaseNumber(NumbersBuffer *numbersBuffer);