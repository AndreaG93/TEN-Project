//
// Created by andrea on 19/11/19.
//

#include <gmp.h>
#include <stdbool.h>
#include "OrderedFactorList.h"
#include "Number.h"
#include "RandomNumber.h"
#include "Common.h"
#include "../IndexCalculusAlgorithm/DLogProblemInstance.h"


void pollardRhoFunction2(__mpz_struct *output, __mpz_struct *input, __mpz_struct *modulo) {

    mpz_pow_ui(output, input, 2);
    mpz_add_ui(output, output, 1);
    mpz_mod(output, output, modulo);
}

__mpz_struct *
getFactorUsingPollardRho2(__mpz_struct *numberToFactorize, unsigned long long maxTrials, NumbersBuffer *numbersBuffer) {

    __mpz_struct *output = allocateNumber();

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 3);

    __mpz_struct *x = buffer[0];;
    __mpz_struct *y = buffer[1];
    __mpz_struct *XMinusY = buffer[2];

    mpz_set_ui(x, 2);
    mpz_set_ui(y, 2);
    mpz_set_si(output, 1);

    for (unsigned long long trial = 0; mpz_cmp_si(output, 1) == 0; trial++) {

        if (trial == maxTrials) {
            freeNumber(output);
            output = NULL;
            break;
        }

        pollardRhoFunction2(x, x, numberToFactorize);
        pollardRhoFunction2(y, y, numberToFactorize);
        pollardRhoFunction2(y, y, numberToFactorize);

        mpz_sub(XMinusY, x, y);
        mpz_abs(XMinusY, XMinusY);
        mpz_gcd(output, XMinusY, numberToFactorize);

        if (mpz_cmp(output, numberToFactorize) == 0) {
            freeNumber(output);
            output = NULL;
            break;
        }
    }

    releaseNumbers(numbersBuffer, 3);
    return output;
}

OrderedFactorList *
factorizeCheckingBSmoothness(__mpz_struct *input, __mpz_struct *smoothnessBound, int maxPollardRhoTrials,
                             NumbersBuffer *numbersBuffer) {

    if (mpz_cmp_ui(input, 0) == 0 || mpz_cmp_ui(input, 1) == 0)
        return NULL;

    OrderedFactorList *output = allocateOrderedFactorList();

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 2);

    __mpz_struct *numberToFactorize = buffer[0];
    __mpz_struct *aux = buffer[1];
    __mpz_struct *factor;

    mpz_set(numberToFactorize, input);

    if (mpz_cmp_ui(numberToFactorize, 0) < 0) {
        mpz_mul_si(aux, numberToFactorize, -2);
        mpz_add(numberToFactorize, aux, numberToFactorize);
    }

    while (mpz_probab_prime_p(numberToFactorize, 15) == 0) {
        factor = getFactorUsingPollardRho2(numberToFactorize, maxPollardRhoTrials, numbersBuffer);
        if (factor == NULL)
            goto ExitFailure;

        while (mpz_probab_prime_p(factor, 15) == 0) {
            factor = getFactorUsingPollardRho2(factor, maxPollardRhoTrials, numbersBuffer);
            if (factor == NULL)
                goto ExitFailure;
        }

        if (mpz_cmp(factor, smoothnessBound) > 0)
            goto ExitFailure;

        mpz_div(numberToFactorize, numberToFactorize, factor);
        insertFactor(output, allocateAndSetNumberFromNumber(factor), allocateAndSetNumberFromULL(1));
    }

    if (mpz_cmp(numberToFactorize, smoothnessBound) > 0)
        goto ExitFailure;
    else
        insertFactor(output, allocateAndSetNumberFromNumber(numberToFactorize), allocateAndSetNumberFromULL(1));

    goto Exit;

    ExitFailure:
    freeOrderedFactorList(output);
    output = NULL;

    Exit:
    releaseNumbers(numbersBuffer, 2);
    return output;
}


void
calcFactorOfPowers(__mpz_struct *input, NumbersBuffer *numbersBuffer, OrderedFactorList *alreadyAllocatedFactorList) {

    __mpz_struct *output = retrieveNumberFromBuffer(numbersBuffer);

    unsigned int exponent = 2;
    while (true) {

        if (mpz_root(output, input, exponent) != 0)
            if (mpz_probab_prime_p(output, 1) != 0)
                break;

        exponent++;
    }

    insertFactor(alreadyAllocatedFactorList, allocateAndSetNumberFromNumber(output),
                 allocateAndSetNumberFromULL(exponent));
    releaseNumber(numbersBuffer);
}

void factorizeUsingTrialDivision(__mpz_struct *numberToFactorize, OrderedFactorList *allocatedFactorList,
                                 NumbersBuffer *numbersBuffer) {


    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 2);

    __mpz_struct *currentPossibleFactor = buffer[0];
    __mpz_struct *remainder = buffer[1];

    mpz_set_ui(currentPossibleFactor, 2);

    while (mpz_cmp_si(numberToFactorize, 1) > 0) {

        mpz_mod(remainder, numberToFactorize, currentPossibleFactor);

        if (mpz_cmp_si(remainder, 0) == 0) {

            __mpz_struct *newFactor = allocateAndSetNumberFromNumber(currentPossibleFactor);
            insertFactor(allocatedFactorList, newFactor, allocateAndSetNumberFromULL(1));

            mpz_div(numberToFactorize, numberToFactorize, currentPossibleFactor);

        } else
            break;
    }

    releaseNumbers(numbersBuffer, 2);
}


OrderedFactorList *factorize(__mpz_struct *input, NumbersBuffer *numbersBuffer) {

    if (mpz_cmp_si(input, 1) == 0)
        return NULL;

    OrderedFactorList *output = allocateOrderedFactorList();

    __mpz_struct *numberToFactorize = retrieveNumberFromBuffer(numbersBuffer);
    mpz_set(numberToFactorize, input);

    factorizeUsingTrialDivision(numberToFactorize, output, numbersBuffer);
    while (mpz_cmp_si(numberToFactorize, 1) > 0) {

        __mpz_struct *factor = getFactorUsingPollardRho2(numberToFactorize, 500, numbersBuffer);
        if (factor == NULL)
            factor = allocateAndSetNumberFromNumber(numberToFactorize);

        if (mpz_probab_prime_p(factor, 15) != 0) {

            mpz_div(numberToFactorize, numberToFactorize, factor);
            insertFactor(output, factor, allocateAndSetNumberFromULL(1));

        } else if (mpz_perfect_power_p(factor) != 0) {

            mpz_div(numberToFactorize, numberToFactorize, factor);
            calcFactorOfPowers(factor, numbersBuffer, output);
        } else
            freeNumber(factor);
    }

    releaseNumber(numbersBuffer);
    return output;
}

void functionxgcd(mpz_t old_r, mpz_t r, mpz_t quotient) {
    mpz_t prov;
    mpz_init(prov);
    mpz_set(prov, r);

    mpz_t temp;
    mpz_init(temp);
    mpz_mul(temp, quotient, prov);

    mpz_sub(r, old_r, temp);
    mpz_set(old_r, prov);

    mpz_clear(prov);
    mpz_clear(temp);
}

int xgcd(mpz_t *result, mpz_t a, mpz_t b, mpz_t compare) {


    mpz_t s;
    mpz_init(s);
    mpz_set_ui(s, 0);
    mpz_t t;
    mpz_init(t);
    mpz_set_ui(t, 1);
    mpz_t r;
    mpz_init(r);
    mpz_set(r, b);
    mpz_t old_s;
    mpz_init(old_s);
    mpz_set_ui(old_s, 1);
    mpz_t old_t;
    mpz_init(old_t);
    mpz_set_ui(old_t, 0);
    mpz_t old_r;
    mpz_init(old_r);
    mpz_set(old_r, a);
    mpz_t temp1;
    mpz_init(temp1);
    mpz_t temp2;
    mpz_init(temp2);
    mpz_t quotient;
    mpz_init(quotient);
    int times = 0;

    while (mpz_cmp(old_r, compare) > 0) {
        //while(time < 3){

        if (mpz_cmp_ui(r, 0) == 0) {
            mpz_clear(s);
            mpz_clear(t);
            mpz_clear(r);
            mpz_clear(old_s);
            mpz_clear(old_t);
            mpz_clear(old_r);
            mpz_clear(temp1);
            mpz_clear(temp2);
            mpz_clear(quotient);
            //printf("return 0 in %d times\n", times);
            return 0;
        }
        mpz_fdiv_q(quotient, old_r, r);
        //printmpz("quotient = ", quotient);
        functionxgcd(old_r, r, quotient);
        functionxgcd(old_s, s, quotient);
        functionxgcd(old_t, t, quotient);

        times++;
    }

    mpz_set(result[0], old_r);
    mpz_set(result[1], old_s);
    mpz_set(result[2], old_t);

    mpz_clear(s);
    mpz_clear(t);
    mpz_clear(r);
    mpz_clear(old_s);
    mpz_clear(old_t);
    mpz_clear(old_r);
    mpz_clear(temp1);
    mpz_clear(temp2);
    mpz_clear(quotient);

    return 1;

}

OrderedFactorList *factorizeCheckingBSmoothnessOptimized(__mpz_struct *input, __mpz_struct *multiplicativeGroup,
                                                         __mpz_struct *magnitudeOfMultiplicativeGroup,
                                                         __mpz_struct *smoothnessBound, NumbersBuffer *numbersBuffer,
                                                         __mpz_struct *sqrtB) {


    mpz_t *result = malloc(3 * sizeof(mpz_t));
    mpz_init(result[0]);
    mpz_init(result[1]);
    mpz_init(result[2]);

    OrderedFactorList *numerator = NULL;
    OrderedFactorList *denominator = NULL;

    if (xgcd(result, multiplicativeGroup, input, magnitudeOfMultiplicativeGroup) == 1) {

        denominator = factorizeCheckingBSmoothness(result[2], smoothnessBound, mpz_get_d(sqrtB), numbersBuffer);
        if (denominator != NULL) {

            numerator = factorizeCheckingBSmoothness(result[0], smoothnessBound, mpz_get_d(sqrtB), numbersBuffer);
            if (numerator != NULL) {

                OrderedFactorList *output = mergeOrderedFactorListUsingOptimization(numerator, denominator);
                freeOrderedFactorList(numerator);
                freeOrderedFactorList(denominator);

                return output;

            } else {
                freeOrderedFactorList(denominator);
                return NULL;
            }
        } else
            return NULL;
    }
    return NULL;
}