//
// Created by andrea on 20/11/19.
//

#include <gmp.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Common.h"
#include "../ApplicationBuffer/ApplicationBuffer.h"
#include "Number.h"

bool isInvertible(ApplicationBuffer *applicationBuffer, mpz_t input, mpz_t modulo) {

    bool output;
    __mpz_struct *gcd = retrieveAuxiliaryNumber(applicationBuffer);

    mpz_gcd(gcd, input, modulo);
    output = (mpz_cmp_ui(gcd, 1) == 0);

    releaseAuxiliaryNumber(applicationBuffer, 1);

    return output;
}

__mpz_struct* getInverseMultiplicative(ApplicationBuffer *applicationBuffer, __mpz_struct *input, __mpz_struct *modulo) {

    __mpz_struct *y_0 = allocateAndSetNumberFromULL(0);

    __mpz_struct *x_0 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *x_1 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *y_1 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *quotient = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *remainder_0 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *remainder_1 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *aux_1 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *aux_2 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *aux_3 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *aux_4 = retrieveAuxiliaryNumber(applicationBuffer);

    mpz_set_si(x_0, 1);
    mpz_set_si(x_1, 0);
    mpz_set_si(y_1, 1);
    mpz_set(remainder_0, modulo);
    mpz_set(remainder_1, input);

    while (mpz_cmp_ui(remainder_1, 0) != 0) {
        mpz_tdiv_q(quotient, remainder_0, remainder_1);

        mpz_mul(aux_1, x_1, quotient);
        mpz_mul(aux_2, y_1, quotient);

        mpz_set(aux_3, x_1);
        mpz_set(aux_4, y_1);

        mpz_sub(x_1, x_0, aux_1);
        mpz_sub(y_1, y_0, aux_2);

        mpz_set(x_0, aux_3);
        mpz_set(y_0, aux_4);

        mpz_mul(aux_1, x_1, modulo);
        mpz_mul(aux_2, y_1, input);

        mpz_set(remainder_0, remainder_1);
        mpz_add(remainder_1, aux_1, aux_2);
    }

    releaseAuxiliaryNumber(applicationBuffer, 10);
    return y_0;
}