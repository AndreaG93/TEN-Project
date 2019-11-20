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


void
factorizeTrialDivision(OrderedFactorList *factorList, __mpz_struct *input, __mpz_struct *aux1, __mpz_struct *aux2) {

    mpz_set_ui(aux1, 2);

    while (mpz_cmp_si(input, 1) > 0) {

        mpz_mod(aux2, input, aux1);
        if (mpz_cmp_si(aux2, 0) == 0) {

            __mpz_struct *newBase = allocateAndSetNumberFromNumber(aux1);
            insertNewFactor(factorList, newBase);

            mpz_div(input, input, aux1);

        } else {
            mpz_add_ui(aux1, aux1, 1);
        }

    }
}


OrderedFactorList *factorize(ApplicationBuffer *applicationBuffer, __mpz_struct *number) {

    OrderedFactorList *output = allocateOrderedFactorList();
    int trial = 0;

    __mpz_struct *x = getAuxiliaryNumber(applicationBuffer, 0);
    __mpz_struct *y = getAuxiliaryNumber(applicationBuffer, 1);
    __mpz_struct *XMinusY = getAuxiliaryNumber(applicationBuffer, 2);
    __mpz_struct *factor = getAuxiliaryNumber(applicationBuffer, 3);
    __mpz_struct *numberToFactorize = getAuxiliaryNumber(applicationBuffer, 4);
    __mpz_struct *actualExponent = getAuxiliaryNumber(applicationBuffer, 5);
    __mpz_struct *aux1 = getAuxiliaryNumber(applicationBuffer, 6);
    __mpz_struct *aux2 = getAuxiliaryNumber(applicationBuffer, 7);

    mpz_set(numberToFactorize, number);

    while (mpz_cmp_ui(numberToFactorize, 1)) {

        mpz_set_ui(x, trial);
        mpz_set_ui(y, trial);
        mpz_set_ui(actualExponent, 0);

        while (1) {

            pollardSRhoFunction(x, numberToFactorize);
            doublePollardSRhoFunction(y, numberToFactorize);

            mpz_sub(XMinusY, x, y);
            mpz_abs(XMinusY, XMinusY);
            mpz_gcd(factor, XMinusY, numberToFactorize);

            if (mpz_cmp_si(factor, 1) > 0) {

                mpz_div(numberToFactorize, numberToFactorize, factor);

                if (mpz_probab_prime_p(factor,15) == 1){

                    __mpz_struct *newBase = allocateAndSetNumberFromNumber(aux1);
                    insertNewFactor(output, newBase);

                } else {
                    factorizeTrialDivision(output, factor, aux1, aux2);
                    break;
                }

            } else if (mpz_cmp(factor, number) == 0) {

                trial++;
                break;
            }
        }
    }

    return output;
}

