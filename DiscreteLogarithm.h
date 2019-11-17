#pragma once

#include <gmp.h>
#include <stdlib.h>
#include <stdbool.h>
#include "GMPCommon.h"

typedef struct {
    __mpz_struct *base;
    __mpz_struct *argument;
    __mpz_struct *modulo;
    __mpz_struct *value;
} DiscreteLogarithm;

DiscreteLogarithm *allocateDiscreteLogarithm() {

    DiscreteLogarithm *output = malloc(sizeof(DiscreteLogarithm));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->argument = allocateAndInitializeNewGMPNumber();
        output->base = allocateAndInitializeNewGMPNumber();
        output->modulo = allocateAndInitializeNewGMPNumber();
        output->value = allocateAndInitializeNewGMPNumber();
    }
    return output;
}

bool checkDiscreteLogarithmCorrectness(DiscreteLogarithm *dLog, __mpz_struct **appBuffer) {

    __mpz_struct *computedDLogArgument = getNumberFromApplicationBuffer(appBuffer, 0);

    mpz_powm(computedDLogArgument, dLog->base, dLog->value, dLog->modulo);

    if (mpz_cmp(computedDLogArgument, dLog->argument) == 0)
        return true;
    else
        return false;
}