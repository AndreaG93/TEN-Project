#pragma once

#include <gmp.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Buffers/NumbersBuffer.h"

typedef struct {

    __mpz_struct *base;
    __mpz_struct *argument;
    __mpz_struct *multiplicativeGroup;
    __mpz_struct *multiplicativeGroupMinusOne;

    __mpz_struct *value;

    __mpz_struct* magnitudeOfMultiplicativeGroup;

} DiscreteLogarithm;

DiscreteLogarithm *allocateDiscreteLogarithm(__mpz_struct *base, __mpz_struct *argument, __mpz_struct *moduloOfMultiplicativeGroup);

bool isCorrect(DiscreteLogarithm *dLog);

void freeDiscreteLogarithm(DiscreteLogarithm *input);