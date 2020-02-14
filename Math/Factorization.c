//
// Created by andrea on 19/11/19.
//

#include <gmp.h>
#include <stdbool.h>
#include "OrderedFactorList.h"
#include "Number.h"
#include "RandomNumber.h"
#include "Common.h"


void pollardRhoFunction2(__mpz_struct *output, __mpz_struct *input, __mpz_struct *modulo) {

    mpz_pow_ui(output, input, 2);
    mpz_add_ui(output, output, 1);
    mpz_mod(output, output, modulo);
}

__mpz_struct *getFactorUsingPollardRho2(__mpz_struct *numberToFactorize, unsigned long long maxTrials, NumbersBuffer *numbersBuffer) {

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

OrderedFactorList *factorizeCheckingBSmoothness(__mpz_struct *input, __mpz_struct *smoothnessBound, int maxPollardRhoTrials, NumbersBuffer *numbersBuffer) {

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





OrderedFactorList *factorizeOptimizedCheckingBSmoothness(__mpz_struct *input, __mpz_struct *modulo, __mpz_struct *smoothnessBound, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator) {

    OrderedFactorList *output = NULL;

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 15);

    __mpz_struct *remainder_0 = buffer[0];
    __mpz_struct *remainder_1 = buffer[1];

    __mpz_struct *s_0 = buffer[2];
    __mpz_struct *t_0 = buffer[3];
    __mpz_struct *s_1 = buffer[4];
    __mpz_struct *t_1 = buffer[5];
    __mpz_struct *quotient = buffer[6];

    __mpz_struct *aux_1 = buffer[7];
    __mpz_struct *aux_2 = buffer[8];
    __mpz_struct *aux_3 = buffer[9];
    __mpz_struct *aux_4 = buffer[10];

    __mpz_struct *numerator = buffer[11];
    __mpz_struct *denominator = buffer[12];

    __mpz_struct *currentNumerator = buffer[13];
    __mpz_struct *currentDenominator = buffer[14];

    bool first = true;

    mpz_set(remainder_0, modulo);
    mpz_set_si(s_0, 1);
    mpz_set_si(t_0, 0);

    mpz_set(remainder_1, input);
    mpz_set_si(s_1, 0);
    mpz_set_si(t_1, 1);

    while (mpz_cmp_ui(remainder_1, 0) != 0) {
        mpz_tdiv_q(quotient, remainder_0, remainder_1);

        mpz_mul(aux_1, s_1, quotient);
        mpz_mul(aux_2, t_1, quotient);

        mpz_set(aux_3, s_1);
        mpz_set(aux_4, t_1);

        mpz_sub(s_1, s_0, aux_1);
        mpz_sub(t_1, t_0, aux_2);

        mpz_set(s_0, aux_3);
        mpz_set(t_0, aux_4);

        mpz_mul(aux_1, s_1, modulo);
        mpz_mul(aux_2, t_1, input);

        mpz_set(remainder_0, remainder_1);
        mpz_add(remainder_1, aux_1, aux_2);

        mpz_mod(currentDenominator, t_1, modulo);
        mpz_mod(currentNumerator, remainder_1, modulo);

        if (mpz_cmp_ui(currentDenominator, 1) != 0 && mpz_cmp_ui(currentDenominator, 0) != 0 && mpz_cmp_ui(currentNumerator, 1) != 0 && mpz_cmp_ui(currentNumerator, 0) != 0) {

            if (first) {
                mpz_set(denominator, currentDenominator);
                mpz_set(numerator, currentNumerator);

                first = false;
            } else if (mpz_cmp(currentDenominator, denominator) < 0 && mpz_cmp(currentNumerator, numerator) < 0) {
                mpz_set(denominator, currentDenominator);
                mpz_set(numerator, currentNumerator);
            }
        }
    }

    if (first) {
        output = factorizeCheckingBSmoothness(input, smoothnessBound, numbersBuffer, randomIntegerGenerator);
    } else {

        OrderedFactorList *numeratorOrderFactorList = factorizeCheckingBSmoothness(numerator, smoothnessBound, numbersBuffer, randomIntegerGenerator);
        OrderedFactorList *denominatorOrderFactorList = factorizeCheckingBSmoothness(denominator, smoothnessBound, numbersBuffer, randomIntegerGenerator);

        if (numeratorOrderFactorList != NULL && denominatorOrderFactorList != NULL) {

            output = mergeOrderedFactorListUsingOptimization(numeratorOrderFactorList, denominatorOrderFactorList);

            freeOrderedFactorList(numeratorOrderFactorList);
            freeOrderedFactorList(denominatorOrderFactorList);

        } else {

            if (numeratorOrderFactorList != NULL)
                freeOrderedFactorList(numeratorOrderFactorList);

            if (denominatorOrderFactorList != NULL)
                freeOrderedFactorList(denominatorOrderFactorList);
        }
    }


    releaseNumbers(numbersBuffer, 15);
    return output;
}









