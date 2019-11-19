#pragma once

#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>

#define AuxiliaryNumbersBufferLength 10

typedef struct {
    __mpz_struct **auxiliaryNumbersBuffer;
} ApplicationBuffer;

ApplicationBuffer *allocateApplicationBuffer();

__mpz_struct *getAuxiliaryNumber(ApplicationBuffer *applicationBuffer, int index);

void freeAuxiliaryNumbersBuffer(ApplicationBuffer *applicationBuffer);