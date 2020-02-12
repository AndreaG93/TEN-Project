#pragma once

#include <gmp.h>
#include "../Buffers/NumbersBuffer.h"
#include "../Math/RandomNumber.h"

typedef struct {
    char *dLogBase;
    char *dLogArgument;
    char *multiplicativeGroup;
    char *smoothnessBound;
} RawUserInput;

typedef struct {
    __mpz_struct *dLogBase;
    __mpz_struct *dLogArgument;
    __mpz_struct *smoothnessBound;
    __mpz_struct *multiplicativeGroup;
    __mpz_struct *maxRandomInteger;

    NumbersBuffer *numbersBuffer;
    RandomIntegerGenerator *randomIntegerGenerator;

} DLogProblemInstanceInput;

DLogProblemInstanceInput *sanitizeRawUserInput(RawUserInput *input, unsigned long maxRandomInteger, unsigned long numbersBufferSize);

__mpz_struct* computeOptimalSmoothnessBound(unsigned long primeNumber);