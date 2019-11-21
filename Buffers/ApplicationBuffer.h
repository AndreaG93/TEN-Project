#pragma once

#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>

#define AuxiliaryNumbersBufferLength 26

typedef struct {
    __mpz_struct **auxiliaryNumbersBuffer;
    unsigned long long currentAvailableNumber;
} ApplicationBuffer;

ApplicationBuffer *allocateApplicationBuffer();

__mpz_struct *retrieveAuxiliaryNumber(ApplicationBuffer *applicationBuffer);

void releaseAuxiliaryNumber(ApplicationBuffer *applicationBuffer, unsigned long long amount);

void deallocateApplicationBuffer(ApplicationBuffer *applicationBuffer);