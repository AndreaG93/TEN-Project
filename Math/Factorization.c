//
// Created by andrea on 19/11/19.
//

#include <gmp.h>
#include <stdbool.h>
#include "Factorization.h"
#include "OrderedFactorList.h"
#include "../ApplicationBuffer/ApplicationBuffer.h"
#include "Number.h"

void pollardSRhoFunction(__mpz_struct *input, __mpz_struct *modulo) {
    mpz_mul(input, input, input);
    mpz_add_ui(input, input, 1);
    mpz_mod(input, input, modulo);
}

void doublePollardSRhoFunction(__mpz_struct *input, __mpz_struct *modulo) {
    pollardSRhoFunction(input, modulo);
    pollardSRhoFunction(input, modulo);
}


OrderedFactorList *factorize(ApplicationBuffer *applicationBuffer, __mpz_struct *number) {

    OrderedFactorList *output = allocateOrderedFactorList();
    bool firstTimeInnerLoop = true;

    __mpz_struct *x = getAuxiliaryNumber(applicationBuffer, 0);
    __mpz_struct *y = getAuxiliaryNumber(applicationBuffer, 1);
    __mpz_struct *XMinusY = getAuxiliaryNumber(applicationBuffer, 2);
    __mpz_struct *factor = getAuxiliaryNumber(applicationBuffer, 3);
    __mpz_struct *numberToFactorize = getAuxiliaryNumber(applicationBuffer, 4);
    __mpz_struct *actualExponent = getAuxiliaryNumber(applicationBuffer, 5);
    __mpz_struct *aux1 = getAuxiliaryNumber(applicationBuffer, 6);
    __mpz_struct *aux2 = getAuxiliaryNumber(applicationBuffer, 7);
    __mpz_struct *aux3 = getAuxiliaryNumber(applicationBuffer, 8);

    mpz_set(numberToFactorize, number);

    while (mpz_cmp_ui(numberToFactorize, 1)) {

        mpz_set_ui(x, 1);
        mpz_set_ui(y, 1);
        mpz_set_ui(actualExponent, 0);

        while (1) {

            pollardSRhoFunction(x, numberToFactorize);
            doublePollardSRhoFunction(y, numberToFactorize);

            mpz_sub(XMinusY, x, y);
            mpz_abs(XMinusY, XMinusY);
            mpz_gcd(factor, XMinusY, numberToFactorize);

            if (mpz_cmp_si(factor, 1) > 0) {

                __mpz_struct *newBase = allocateAndSetNumber(factor);
                insertNewFactor(output, newBase);

                break;
            }


            mpz_div(numberToFactorize, aux2, aux3);
        }
    }

    return output;
}