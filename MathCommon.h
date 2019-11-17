#pragma once

#include <gmp.h>
#include <stdbool.h>
#include "GMPCommon.h"
#include "DataStructures/UnorderedList.h"

void PollardSRhoFunction(mpz_t x, mpz_t modulo) {
    mpz_mul(x, x, x);
    mpz_add_ui(x, x, 2);
    mpz_mod(x, x, modulo);
}

bool isInvertible(mpz_t input, mpz_t modulo, __mpz_struct **appBuffer) {

    __mpz_struct *gcd = getNumberFromApplicationBuffer(appBuffer, 0);

    mpz_gcd(gcd, input, modulo);
    return (mpz_cmp_ui(gcd, 1) == 0);
}

bool isBSmooth(mpz_t input, mpz_t smoothnessBound) {
/*
    FactorList* factorList;

    if (factorList->length != 0 && mpz_cmp(factorList->tail->data, smoothnessBound) <= 0)
        return true;
    else
        return false;
        */
    return false;
}

mpz_ptr getInverseMultiplicative(__mpz_struct *input, __mpz_struct *modulo, __mpz_struct **appBuffer) {

    __mpz_struct *y_0 = malloc(sizeof(mpz_ptr));
    if (y_0 == NULL)
        exit(EXIT_FAILURE);
    else
        mpz_init_set_si(y_0, 0);

    __mpz_struct *x_0 = getNumberFromApplicationBuffer(appBuffer, 0);
    __mpz_struct *x_1 = getNumberFromApplicationBuffer(appBuffer, 1);
    __mpz_struct *y_1 = getNumberFromApplicationBuffer(appBuffer, 2);
    __mpz_struct *quotient = getNumberFromApplicationBuffer(appBuffer, 3);
    __mpz_struct *remainder_0 = getNumberFromApplicationBuffer(appBuffer, 4);
    __mpz_struct *remainder_1 = getNumberFromApplicationBuffer(appBuffer, 5);
    __mpz_struct *aux_1 = getNumberFromApplicationBuffer(appBuffer, 6);
    __mpz_struct *aux_2 = getNumberFromApplicationBuffer(appBuffer, 7);
    __mpz_struct *aux_3 = getNumberFromApplicationBuffer(appBuffer, 8);
    __mpz_struct *aux_4 = getNumberFromApplicationBuffer(appBuffer, 9);

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

    return y_0;
}

UnorderedList *PollardSRhoAlgorithm(mpz_t input, __mpz_struct **appBuffer) {

    UnorderedList *output = allocUnorderedList();

    __mpz_struct *x = getNumberFromApplicationBuffer(appBuffer, 0);
    __mpz_struct *y = getNumberFromApplicationBuffer(appBuffer, 0);
    __mpz_struct *XMinusY = getNumberFromApplicationBuffer(appBuffer, 0);
    __mpz_struct *factor = getNumberFromApplicationBuffer(appBuffer, 0);

    while (1) {

        mpz_set_ui(x, 1);
        mpz_set_ui(y, 1);

        while (1) {

            PollardSRhoFunction(x, input);
            PollardSRhoFunction(y, input);
            PollardSRhoFunction(y, input);

            mpz_sub(XMinusY, x, y);
            mpz_gcd(factor, XMinusY, input);

            if (mpz_cmp_si(factor, 1) != 0) {

                __mpz_struct *foundFactor = allocateAndInitializeNewGMPNumber();
                mpz_init_set(foundFactor, factor);

                insert(output, foundFactor);
                break;
            } else if (mpz_cmp(factor, input) == 0)
                break;
        }

        mpz_div(input, input, factor);
        if (mpz_cmp_si(input, 1) == 0) {
            break;
        }
    }

    return output;
}