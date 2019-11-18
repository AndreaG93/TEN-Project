#pragma once

#include <stddef.h>
#include <gmp.h>
#include <stdlib.h>
#include "GMPCommon.h"
#include "MathCommon.h"
#include "ControlBlock.h"

void getInverseMultiplicative_test() {

    __mpz_struct *input = allocateAndInitializeNewGMPNumber();
    __mpz_struct *modulo = allocateAndInitializeNewGMPNumber();
    __mpz_struct *inverse;

    if (mpz_set_str(input, "5", 10) != 0)
        exit(EXIT_FAILURE);

    if (mpz_set_str(modulo, "178", 10) != 0)
        exit(EXIT_FAILURE);

    inverse = getInverseMultiplicative(input, modulo, controlBlock->auxiliaryNumberBuffer);
    if ((mpz_cmp_si(inverse,-71)) != 0)
        exit(EXIT_FAILURE);
}

void generatePrimeDatabase_test(){
    generatePrimeDatabase();
}

void isInvertible_test() {

    __mpz_struct *input = allocateAndInitializeNewGMPNumber();
    __mpz_struct *modulo = allocateAndInitializeNewGMPNumber();

    if (mpz_set_str(input, "5", 10) != 0)
        exit(EXIT_FAILURE);

    if (mpz_set_str(modulo, "178", 10) != 0)
        exit(EXIT_FAILURE);

    if (isInvertible(input, modulo, controlBlock->auxiliaryNumberBuffer) != true)
        exit(EXIT_FAILURE);
    else
        freeAuxiliaryBuffer(controlBlock->auxiliaryNumberBuffer);
}
