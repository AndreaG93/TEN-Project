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


void pollardRhoFunction(__mpz_struct *output, __mpz_struct *input, __mpz_struct *modulo) {

    mpz_pow_ui(output, input, 2);
    mpz_add_ui(output, output, 1);
    mpz_mod(output, output, modulo);
}

__mpz_struct *
getFactorUsingPollardRho(__mpz_struct *numberToFactorize, unsigned long long maxTrials, NumbersBuffer *numbersBuffer) {

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

        pollardRhoFunction(x, x, numberToFactorize);
        pollardRhoFunction(y, y, numberToFactorize);
        pollardRhoFunction(y, y, numberToFactorize);

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
        factor = getFactorUsingPollardRho(numberToFactorize, maxPollardRhoTrials, numbersBuffer);
        if (factor == NULL)
            goto ExitFailure;

        while (mpz_probab_prime_p(factor, 15) == 0) {
            factor = getFactorUsingPollardRho(factor, maxPollardRhoTrials, numbersBuffer);
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

        __mpz_struct *factor = getFactorUsingPollardRho(numberToFactorize, 500, numbersBuffer);
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

void extendedEuclidSupport(mpz_t old_r, mpz_t r, mpz_t quotient, NumbersBuffer *numbersBuffer) {

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 2);

    __mpz_struct *temp1 = buffer[0];
    __mpz_struct *temp2 = buffer[1];

    mpz_set(temp1, r);
    mpz_mul(temp2, quotient, temp1);

    mpz_sub(r, old_r, temp2);
    mpz_set(old_r, temp1);

    releaseNumbers(numbersBuffer, 2);
}

int extendedEuclid(__mpz_struct **result, mpz_t a, mpz_t b, mpz_t compare, NumbersBuffer *numbersBuffer) {

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 7);

    __mpz_struct *s = buffer[0];
    __mpz_struct *t = buffer[1];
    __mpz_struct *r = buffer[2];
    __mpz_struct *old_s = buffer[3];
    __mpz_struct *old_t = buffer[4];
    __mpz_struct *old_r = buffer[5];
    __mpz_struct *quotient = buffer[6];

    mpz_set_ui(old_s, 1);
    mpz_set_ui(old_t, 0);
    mpz_set(old_r, a);

    mpz_set_ui(s, 0);
    mpz_set_ui(t, 1);
    mpz_set(r, b);

    int times = 0;

    while (mpz_cmp(old_r, compare) > 0) {

        if (mpz_cmp_ui(r, 0) == 0) {

            releaseNumbers(numbersBuffer, 9);
            return 0;
        }

        mpz_fdiv_q(quotient, old_r, r);

        extendedEuclidSupport(old_r, r, quotient, numbersBuffer);
        extendedEuclidSupport(old_s, s, quotient, numbersBuffer);
        extendedEuclidSupport(old_t, t, quotient, numbersBuffer);

        times++;
    }

    mpz_set(result[0], old_r);
    mpz_set(result[1], old_t);

    releaseNumbers(numbersBuffer, 7);

    return 1;
}

OrderedFactorList *factorizeCheckingBSmoothnessOptimized(__mpz_struct *input, __mpz_struct *multiplicativeGroup,
                                                         __mpz_struct *magnitudeOfMultiplicativeGroup,
                                                         __mpz_struct *smoothnessBound, NumbersBuffer *numbersBuffer,
                                                         __mpz_struct *sqrtB) {

    __mpz_struct **result = allocateNumbersArray(2, true);

    OrderedFactorList *numerator = NULL;
    OrderedFactorList *denominator = NULL;

    if (extendedEuclid(result, multiplicativeGroup, input, magnitudeOfMultiplicativeGroup, numbersBuffer) == 1) {

        denominator = factorizeCheckingBSmoothness(result[1], smoothnessBound, mpz_get_d(sqrtB), numbersBuffer);
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