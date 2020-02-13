//
// Created by andrea on 19/11/19.
//

#include <gmp.h>
#include <stdbool.h>
#include "OrderedFactorList.h"
#include "Number.h"
#include "RandomNumber.h"
#include "Common.h"

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


bool factorizeUsingTrialDivisionCheckingBSmoothness(__mpz_struct *numberToFactorize, __mpz_struct *smoothnessBound, OrderedFactorList *allocatedFactorList, NumbersBuffer *numbersBuffer) {

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

__mpz_struct *getFactorUsingPollardRho(__mpz_struct *numberToFactorize, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator) {

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

            output = allocateAndSetNumberFromNumber(possibleFactor);
            break;
        }
    }

    freeNumber(x);
    releaseNumbers(numbersBuffer, 4);
    return output;
}

OrderedFactorList *factorizeCheckingBSmoothness(__mpz_struct *input, __mpz_struct *smoothnessBound, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator) {

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

        } else {
            isNumberToFactorizeBSmooth = factorizeUsingTrialDivisionCheckingBSmoothness(factor, smoothnessBound, output, numbersBuffer);
            freeNumber(factor);
        }


        if (isNumberToFactorizeBSmooth == false) {
            freeOrderedFactorList(output);
            output = NULL;
            break;
        }
    }

    releaseNumber(numbersBuffer);
    return output;
}

bool checkIfBSmooth(__mpz_struct *input, __mpz_struct *smoothnessBound, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator) {

    bool output = true;

    if (mpz_cmp_si(input, 1) == 0)
        return false;

    __mpz_struct *numberToFactorize = retrieveNumberFromBuffer(numbersBuffer);
    mpz_set(numberToFactorize, input);

    while (mpz_probab_prime_p(numberToFactorize, 15) == 0) {

        __mpz_struct *factor = getFactorUsingPollardRho(numberToFactorize, numbersBuffer, randomIntegerGenerator);
        if (factor == NULL) {
            output = false;
            break;

        } else {

            if (mpz_cmp(factor, smoothnessBound) > 0) {
                output = false;
                freeNumber(factor);
                break;
            }

            mpz_div(numberToFactorize, numberToFactorize, factor);
            freeNumber(factor);
        }
    }

    if (mpz_cmp(numberToFactorize, smoothnessBound) > 0)
        output = false;

    releaseNumber(numbersBuffer);
    return output;
}

void SubForMulX() {

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

    mpz_set(remainder_0, input);
    mpz_set_si(s_0, 1);
    mpz_set_si(t_0, 0);

    mpz_set(remainder_1, modulo);
    mpz_set_si(s_1, 0);
    mpz_set_si(t_1, 1);

    while (mpz_cmp_ui(remainder_1, 0) != 0) {
        mpz_tdiv_q(quotient, remainder_0, remainder_1);

        mpz_mul(aux_1, remainder_1, quotient);
        mpz_set(aux_2, remainder_0);
        mpz_set(remainder_0, remainder_1);
        mpz_sub(remainder_1, aux_2, aux_1);

        mpz_mul(aux_1, s_1, quotient);
        mpz_set(aux_2, s_0);
        mpz_set(s_0, s_1);
        mpz_sub(s_1, aux_2, aux_1);

        mpz_mul(aux_1, t_1, quotient);
        mpz_set(aux_2, t_0);
        mpz_set(t_0, t_1);
        mpz_sub(t_1, aux_2, aux_1);

        mpz_mod(currentDenominator, t_0, modulo);
        mpz_mod(currentNumerator, remainder_1, modulo);

        if (checkIfBSmooth(currentNumerator,smoothnessBound, numbersBuffer, randomIntegerGenerator) && checkIfBSmooth(currentDenominator,smoothnessBound, numbersBuffer, randomIntegerGenerator))
            gmp_fprintf(stderr,"--> Numera (%Zd) e (%Zd) is bsmooth\n", currentNumerator, currentDenominator);
        else
            gmp_fprintf(stderr,"--> Numera (%Zd) e (%Zd) NON SONO bsmooth\n", currentNumerator, currentDenominator);


        if (mpz_cmp_ui(currentDenominator, 1) != 0 && mpz_cmp_ui(currentDenominator, 0) != 0 && mpz_cmp_ui(currentNumerator, 1) != 0 && mpz_cmp_ui(currentNumerator, 0) != 0) {

            if (first) {
                mpz_set(denominator, currentDenominator);
                mpz_set(numerator, currentNumerator);

                first = false;
            } else if (mpz_cmp(currentDenominator, denominator) <= 0 && mpz_cmp(currentNumerator, numerator) <= 0) {
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









