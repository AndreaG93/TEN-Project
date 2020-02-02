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

void
factorizeUsingTrialDivision(__mpz_struct *input, OrderedFactorList *allocatedFactorList, NumbersBuffer *numbersBuffer) {

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 3);

    __mpz_struct *possibleFactor = buffer[0];
    __mpz_struct *squareOfPossibleFactor = buffer[1];
    __mpz_struct *remainder = buffer[2];

    mpz_set_ui(possibleFactor, 3);

    while (mpz_cmp_si(input, 1) > 0) {

        mpz_mod(remainder, input, possibleFactor);

        if (mpz_cmp_si(remainder, 0) == 0) {

            __mpz_struct *newFactor = allocateAndSetNumberFromNumber(possibleFactor);
            insertNewFactor(allocatedFactorList, newFactor);

            mpz_div(input, input, possibleFactor);

        } else {

            mpz_mul(squareOfPossibleFactor, possibleFactor, possibleFactor);

            if (mpz_cmp(squareOfPossibleFactor, input) <= 0)
                mpz_add_ui(possibleFactor, possibleFactor, 2);
            else
                break;
        }
    }

    releaseNumbers(numbersBuffer, 3);
}

void factorizeUsingTrialDivisionWhenInputIsEven(__mpz_struct *input, OrderedFactorList *allocatedFactorList,
                                                NumbersBuffer *numbersBuffer) {

    __mpz_struct *remainder = retrieveNumberFromBuffer(numbersBuffer);

    while (mpz_cmp_si(input, 1) > 0) {

        mpz_mod_ui(remainder, input, 2);

        if (mpz_cmp_si(remainder, 0) == 0) {

            __mpz_struct *newFactor = allocateAndSetNumberFromULL(2);
            insertNewFactor(allocatedFactorList, newFactor);

            mpz_div_ui(input, input, 2);

        } else
            break;
    }

    releaseNumber(numbersBuffer);
}


__mpz_struct *getFactorUsingPollardRho(__mpz_struct *input, __mpz_struct *modulo, NumbersBuffer *numbersBuffer,
                                       RandomIntegerGenerator *randomIntegerGenerator) {

    __mpz_struct *output = NULL;

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 4);

    __mpz_struct *x = buffer[0];
    __mpz_struct *y = buffer[1];
    __mpz_struct *XMinusY = buffer[2];
    __mpz_struct *possibleFactor = buffer[3];

    selectUniformlyDistributedRandomInteger(randomIntegerGenerator, x);
    mpz_set(y, x);
    mpz_set_si(possibleFactor, 1);

    while (mpz_cmp_si(possibleFactor, 1) == 0) {

        pollardRhoFunction(x, x, modulo);
        pollardRhoFunction(y, y, modulo);
        pollardRhoFunction(y, y, modulo);

        mpz_sub(XMinusY, x, y);
        mpz_abs(XMinusY, XMinusY);
        mpz_gcd(possibleFactor, XMinusY, input);

        if (mpz_cmp(possibleFactor, modulo) == 0) {
            break;
        } else if (mpz_cmp_si(possibleFactor, 1) > 0) {

            output = allocateNumber();
            mpz_set(output, possibleFactor);

            break;
        }
    }

    releaseNumbers(numbersBuffer, 4);
    return output;
}


OrderedFactorList *factorize(__mpz_struct *input, __mpz_struct *modulo, NumbersBuffer *numbersBuffer,
                             RandomIntegerGenerator *randomIntegerGenerator) {

    OrderedFactorList *output = NULL;

    if (mpz_cmp_si(input, 1) > 0) {

        output = allocateOrderedFactorList();

        factorizeUsingTrialDivisionWhenInputIsEven(input, output, numbersBuffer);

        while (mpz_cmp_si(input, 1) > 0) {

            if (mpz_probab_prime_p(input, 15) == 2) {

                insertNewFactor(output, input);
                break;

            } else {

                __mpz_struct *factor = getFactorUsingPollardRho(input, modulo, numbersBuffer, randomIntegerGenerator);
                mpz_div(input, input, factor);

                if (mpz_probab_prime_p(input, 15) == 2)
                    insertNewFactor(output, factor);
                else
                    factorizeUsingTrialDivision(factor, output, numbersBuffer);
            }
        }
    }
    return output;
}


OrderedFactorList *factorizeOptimized(__mpz_struct *input, __mpz_struct *modulo, NumbersBuffer *numbersBuffer,
                                      RandomIntegerGenerator *randomIntegerGenerator) {

    size_t inputNumberSize = mpz_sizeinbase(input, 10);
    inputNumberSize = inputNumberSize / 2;
    size_t zSize;
    size_t xSize;

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 8);


    __mpz_struct *remainder_0 = buffer[0];
    __mpz_struct *remainder_1 = allocateNumber();

    __mpz_struct *s_0 = buffer[1];
    __mpz_struct *t_0 = buffer[2];
    __mpz_struct *s_1 = buffer[3];
    __mpz_struct *t_1 = allocateNumber();
    __mpz_struct *quotient = buffer[4];

    __mpz_struct *aux_1 = buffer[5];
    __mpz_struct *aux_2 = buffer[6];
    __mpz_struct *aux_3 = buffer[7];
    __mpz_struct *aux_4 = buffer[8];
    __mpz_struct *aux_5 = buffer[9];

    mpz_mod(input, input, modulo);

    mpz_set_si(s_0, 1);
    mpz_set_si(t_0, 0);
    mpz_set(remainder_0, modulo);

    mpz_set_si(s_1, 0);
    mpz_set_si(t_1, 1);
    mpz_set(remainder_1, input);

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

        mpz_mod(aux_5, t_1, modulo);
        xSize = mpz_sizeinbase(aux_5, 10);
        zSize = mpz_sizeinbase(remainder_1, 10);

        if (xSize >= (inputNumberSize) / 2) {


        }
    }

    return NULL;
}









/*
size_t inputNumberSize = mpz_sizeinbase(number, 10);
inputNumberSize = inputNumberSize / 2;
size_t zSize;
size_t xSize;

__mpz_struct** buffer = retrieveNumbersFromBuffer(numbersBuffer, 8);


__mpz_struct *remainder_0 = buffer[0];
__mpz_struct *remainder_1 = allocateNumber();

__mpz_struct *s_0 = buffer[1];
__mpz_struct *t_0 = buffer[2];
__mpz_struct *s_1 = buffer[3];
__mpz_struct *t_1 = allocateNumber();
__mpz_struct *quotient = buffer[4];

__mpz_struct *aux_1 = buffer[5];
__mpz_struct *aux_2 = buffer[6];
__mpz_struct *aux_3 = buffer[7];
__mpz_struct *aux_4 = buffer[8];

__mpz_struct *aux_5 = buffer[9];

__mpz_struct *input = buffer[10];
mpz_mod(input, number, modulo);

mpz_set_si(s_0, 1);
mpz_set_si(t_0, 0);
mpz_set(remainder_0, modulo);

mpz_set_si(s_1, 0);
mpz_set_si(t_1, 1);
mpz_set(remainder_1, input);

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

    mpz_mod(aux_5, t_1, modulo);
    xSize = mpz_sizeinbase(aux_5, 10);
    zSize = mpz_sizeinbase(remainder_1, 10);

    if (xSize >= (inputNumberSize) / 2) {

        __mpz_struct *bound = allocateAndSetNumberFromString("4422347445564");


        OrderedFactorList *leftOrderFactorList = allocateOrderedFactorList();
        factorizeUsingTrialDivision(remainder_1, leftOrderFactorList, numbersBuffer);

        OrderedFactorList *rightOrderFactorList = allocateOrderedFactorList();
        factorizeUsingTrialDivision(t_1, rightOrderFactorList, numbersBuffer);

        OrderedFactorListNode *currentLeftOrderFactorListNode = leftOrderFactorList->head;


        while (currentLeftOrderFactorListNode != NULL) {
            gmp_printf("%Zd ^ %Zd\n", currentLeftOrderFactorListNode->factor->base,
                       currentLeftOrderFactorListNode->factor->exponent);
            currentLeftOrderFactorListNode = currentLeftOrderFactorListNode->next_node;
        }

        fprintf(stdout, "____");

        currentLeftOrderFactorListNode = rightOrderFactorList->head;

        while (currentLeftOrderFactorListNode != NULL) {
            gmp_printf("%Zd ^ %Zd\n", currentLeftOrderFactorListNode->factor->base,
                       currentLeftOrderFactorListNode->factor->exponent);
            currentLeftOrderFactorListNode = currentLeftOrderFactorListNode->next_node;
        }

        exit(6);
    }


}


mpz_mod(t_0, t_0, modulo);
*/


/*
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
    return output;
}

*/