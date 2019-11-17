#pragma once

#include <stddef.h>
#include <gmp.h>
#include <stdlib.h>
#include "GMPCommon.h"
#include "MathCommon.h"

void getInverseMultiplicative_test() {

    __mpz_struct **appBuffer = allocateApplicationBuffer();
    __mpz_struct *input = allocateAndInitializeNewGMPNumber();
    __mpz_struct *modulo = allocateAndInitializeNewGMPNumber();
    __mpz_struct *inverse;

    if (mpz_set_str(input, "5", 10) != 0)
        exit(EXIT_FAILURE);

    if (mpz_set_str(modulo, "178", 10) != 0)
        exit(EXIT_FAILURE);

    inverse = getInverseMultiplicative(input, modulo, appBuffer);
    if ((mpz_cmp_si(inverse,-71)) == 0)
        freeApplicationBuffer(appBuffer);
    else {
        exit(EXIT_FAILURE);
    }
}

void isInvertible_test() {
    __mpz_struct **appBuffer = allocateApplicationBuffer();
    __mpz_struct *input = allocateAndInitializeNewGMPNumber();
    __mpz_struct *modulo = allocateAndInitializeNewGMPNumber();

    if (mpz_set_str(input, "5", 10) != 0)
        exit(EXIT_FAILURE);

    if (mpz_set_str(modulo, "178", 10) != 0)
        exit(EXIT_FAILURE);

    if (isInvertible(input, modulo, appBuffer) != true)
        exit(EXIT_FAILURE);
    else
        freeApplicationBuffer(appBuffer);
}
