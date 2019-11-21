#pragma once

#include <gmp.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Buffers/ApplicationBuffer.h"

typedef struct {
    __mpz_struct *base;
    __mpz_struct *argument;
    __mpz_struct *moduloOfMultiplicativeGroup;
    __mpz_struct *value;
} DiscreteLogarithm;

DiscreteLogarithm *allocateDiscreteLogarithm(char *base, char *argument, __mpz_struct* moduloOfMultiplicativeGroup);

bool isCorrect(DiscreteLogarithm *dLog, ApplicationBuffer *appBuffer);