//
// Created by andrea on 19/11/19.
//

#include <gmp.h>
#include <stdbool.h>
#include <time.h>
#include "Factorization.h"
#include "OrderedFactorList.h"
#include "Number.h"
#include "RandomNumber.h"
#include "Common.h"

#define INFINITY_TRIALS -1

void pollardRhoFunction(__mpz_struct *output, __mpz_struct *input, __mpz_struct *modulo) {

    mpz_mul(output, input, input);
    mpz_add_ui(output, output, 2);
    mpz_mod(output, output, modulo);
}

bool isBSmooth(OrderedFactorList *factorList, __mpz_struct *smoothnessBound) {

    if (factorList->tail != NULL && mpz_cmp(factorList->tail->factor->base, smoothnessBound) <= 0)
        return true;
    else
        return false;
}

bool factorizeUsingTrialDivisionCheckingBSmoothness(__mpz_struct *numberToFactorize, __mpz_struct *smoothnessBound,
                                                    OrderedFactorList *allocatedFactorList,
                                                    NumbersBuffer *numbersBuffer) {

    bool isNumberToFactorizeBSmooth = true;

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 3);

    __mpz_struct *currentPossibleFactor = buffer[0];
    __mpz_struct *squareOfCurrentPossibleFactor = buffer[1];
    __mpz_struct *remainder = buffer[2];

    mpz_set_ui(currentPossibleFactor, 2);

    while (mpz_cmp_si(numberToFactorize, 1) > 0) {

        mpz_mod(remainder, numberToFactorize, currentPossibleFactor);

        if (mpz_cmp_si(remainder, 0) == 0) {

            __mpz_struct *newFactor = allocateAndSetNumberFromNumber(currentPossibleFactor);
            insertFactor(allocatedFactorList, newFactor, allocateAndSetNumberFromULL(1));

            mpz_div(numberToFactorize, numberToFactorize, currentPossibleFactor);

            if (smoothnessBound != NULL)
                if (isBSmooth(allocatedFactorList, smoothnessBound) == false) {
                    isNumberToFactorizeBSmooth = false;
                    break;
                }

        } else
            break;
    }

    mpz_set_ui(currentPossibleFactor, 3);

    while (mpz_cmp_si(numberToFactorize, 1) > 0) {

        mpz_mod(remainder, numberToFactorize, currentPossibleFactor);

        if (mpz_cmp_si(remainder, 0) == 0) {

            __mpz_struct *newFactor = allocateAndSetNumberFromNumber(currentPossibleFactor);
            insertFactor(allocatedFactorList, newFactor, allocateAndSetNumberFromULL(1));

            mpz_div(numberToFactorize, numberToFactorize, currentPossibleFactor);

            if (smoothnessBound != NULL)
                if (isBSmooth(allocatedFactorList, smoothnessBound) == false) {
                    isNumberToFactorizeBSmooth = false;
                    break;
                }

        } else {

            mpz_mul(squareOfCurrentPossibleFactor, currentPossibleFactor, currentPossibleFactor);

            if (mpz_cmp(squareOfCurrentPossibleFactor, numberToFactorize) <= 0)
                mpz_add_ui(currentPossibleFactor, currentPossibleFactor, 2);
            else
                mpz_set(currentPossibleFactor, numberToFactorize);
        }
    }

    releaseNumbers(numbersBuffer, 3);
    return isNumberToFactorizeBSmooth;
}

__mpz_struct *getFactorUsingPollardRho(__mpz_struct *numberToFactorize, NumbersBuffer *numbersBuffer,
                                       RandomIntegerGenerator *randomIntegerGenerator) {

    __mpz_struct *output = NULL;

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 4);

    __mpz_struct *x;
    __mpz_struct *y = buffer[0];
    __mpz_struct *XMinusY = buffer[1];
    __mpz_struct *possibleFactor = buffer[2];
    __mpz_struct *maxTrials = buffer[3];

    mpz_sqrt(maxTrials, numberToFactorize);
    mpz_sqrt(maxTrials, maxTrials);

    x = selectUniformlyDistributedRandomInteger(randomIntegerGenerator);
    mpz_set(y, x);
    mpz_set_si(possibleFactor, 1);

    for (; mpz_cmp_si(possibleFactor, 1) == 0 && mpz_cmp_ui(maxTrials, 0) != 0; mpz_sub_ui(maxTrials, maxTrials, 1)) {

        pollardRhoFunction(x, x, numberToFactorize);
        pollardRhoFunction(y, y, numberToFactorize);
        pollardRhoFunction(y, y, numberToFactorize);

        mpz_sub(XMinusY, x, y);
        mpz_abs(XMinusY, XMinusY);
        mpz_gcd(possibleFactor, XMinusY, numberToFactorize);

        if (mpz_cmp(possibleFactor, numberToFactorize) == 0)
            break;
        else if (mpz_cmp_si(possibleFactor, 1) > 0) {

            output = allocateNumber();
            mpz_set(output, possibleFactor);

            break;
        }
    }

    freeNumber(x);
    releaseNumbers(numbersBuffer, 4);
    return output;
}

OrderedFactorList *
factorizeCheckingBSmoothness(__mpz_struct *input, __mpz_struct *smoothnessBound, NumbersBuffer *numbersBuffer,
                             RandomIntegerGenerator *randomIntegerGenerator) {

    if (mpz_cmp_si(input, 1) == 0)
        return NULL;

    OrderedFactorList *output = allocateOrderedFactorList();
    bool isNumberToFactorizeBSmooth = true;
    __mpz_struct *numberToFactorize = retrieveNumberFromBuffer(numbersBuffer);
    mpz_set(numberToFactorize, input);

    while (mpz_cmp_si(numberToFactorize, 1) > 0) {

        __mpz_struct *factor = getFactorUsingPollardRho(numberToFactorize, numbersBuffer, randomIntegerGenerator);
        if (factor == NULL)
            factor = allocateAndSetNumberFromNumber(numberToFactorize);

        mpz_div(numberToFactorize, numberToFactorize, factor);

        if (mpz_probab_prime_p(factor, 1) != 0) {
            insertFactor(output, factor, allocateAndSetNumberFromULL(1));

            if (smoothnessBound != NULL)
                isNumberToFactorizeBSmooth = isBSmooth(output, smoothnessBound);

        } else
            isNumberToFactorizeBSmooth = factorizeUsingTrialDivisionCheckingBSmoothness(factor, smoothnessBound, output,
                                                                                        numbersBuffer);


        if (isNumberToFactorizeBSmooth == false) {
            freeOrderedFactorList(output);
            output = NULL;
            break;
        }
    }

    releaseNumber(numbersBuffer);
    return output;
}

OrderedFactorList *
factorizeOptimizedCheckingBSmoothness(__mpz_struct *input, __mpz_struct *modulo, __mpz_struct *smoothnessBound,
                                      NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator) {

    OrderedFactorList *output = NULL;

    size_t inputNumberSize = mpz_sizeinbase(input, 10);
    inputNumberSize = inputNumberSize / 2;

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 13);

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

        mpz_mod(denominator, t_1, modulo);
        mpz_mod(numerator, remainder_1, modulo);

        if (mpz_sizeinbase(denominator, 10) <= inputNumberSize && mpz_sizeinbase(numerator, 10) <= inputNumberSize &&
            mpz_cmp_ui(denominator, 1) != 0 && mpz_cmp_ui(denominator, 0) != 0 &&
            mpz_cmp_ui(numerator, 1) != 0 && mpz_cmp_ui(numerator, 0) != 0) {

            OrderedFactorList *numeratorOrderFactorList = factorizeCheckingBSmoothness(numerator, smoothnessBound,
                                                                                       numbersBuffer,
                                                                                       randomIntegerGenerator);
            OrderedFactorList *denominatorOrderFactorList = factorizeCheckingBSmoothness(denominator, smoothnessBound,
                                                                                         numbersBuffer,
                                                                                         randomIntegerGenerator);


            if (numeratorOrderFactorList != NULL && denominatorOrderFactorList != NULL)
                output = mergeOrderedFactorListUsingOptimization(numeratorOrderFactorList, denominatorOrderFactorList);

            if (numeratorOrderFactorList != NULL)
                freeOrderedFactorList(numeratorOrderFactorList);

            if (denominatorOrderFactorList != NULL)
                freeOrderedFactorList(denominatorOrderFactorList);

            break;
        }
    }

    if (output == NULL)
        output = factorizeCheckingBSmoothness(input, smoothnessBound, numbersBuffer, randomIntegerGenerator);

    releaseNumbers(numbersBuffer, 13);
    return output;
}









