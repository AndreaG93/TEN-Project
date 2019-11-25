//
// Created by andrea on 19/11/19.
//

#include <gmp.h>
#include <stdbool.h>
#include "Factorization.h"
#include "OrderedFactorList.h"
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

bool isBSmooth(OrderedFactorList *factorList, __mpz_struct *smoothnessBound) {

    if (factorList->tail != NULL && mpz_cmp(factorList->tail->factor->base, smoothnessBound) <= 0)
        return true;
    else
        return false;
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


OrderedFactorList *factorizeCheckingBSmoothness(ApplicationBuffer *applicationBuffer, __mpz_struct *number,
                                                __mpz_struct *smoothnessBound) {

    if (mpz_cmp_ui(number, 1) == 0)
        return NULL;

    OrderedFactorList *output = allocateOrderedFactorList();
    int trial = 0;

    __mpz_struct *x = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *y = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *XMinusY = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *factor = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *numberToFactorize = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *actualExponent = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *aux1 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *aux2 = retrieveAuxiliaryNumber(applicationBuffer);

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

                if (mpz_probab_prime_p(factor, 15) == 1) {

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

        if (isBSmooth(output, smoothnessBound) == false) {

            deallocateOrderedFactorList(output);
            releaseAuxiliaryNumber(applicationBuffer, 8);
            return NULL;
        }
    }

    releaseAuxiliaryNumber(applicationBuffer, 8);

    if (output != NULL)
        if (output->head == NULL)
            fprintf(stderr, "UN problema\n");


    return output;
}

