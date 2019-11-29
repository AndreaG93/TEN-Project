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


void factorize(ApplicationBuffer *applicationBuffer, __mpz_struct *number, __mpz_struct *modulo) {

    size_t inputNumberSize = mpz_sizeinbase(number, 10);
    inputNumberSize = inputNumberSize / 2;
    size_t zSize;
    size_t xSize;

    __mpz_struct *remainder_0 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *remainder_1 = allocateNumber();

    __mpz_struct *s_0 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *t_0 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *s_1 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *t_1 = allocateNumber();
    __mpz_struct *quotient = retrieveAuxiliaryNumber(applicationBuffer);

    __mpz_struct *aux_1 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *aux_2 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *aux_3 = retrieveAuxiliaryNumber(applicationBuffer);
    __mpz_struct *aux_4 = retrieveAuxiliaryNumber(applicationBuffer);

    __mpz_struct *aux_5 = retrieveAuxiliaryNumber(applicationBuffer);

    __mpz_struct *input = retrieveAuxiliaryNumber(applicationBuffer);
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

            __mpz_struct* bound = allocateAndSetNumberFromString("4422347445564");
            releaseAuxiliaryNumber(applicationBuffer, 11);
            OrderedFactorList* leftOrderFactorList = factorizeCheckingBSmoothness(applicationBuffer, remainder_1, bound);
            OrderedFactorList* rightOrderFactorList = factorizeCheckingBSmoothness(applicationBuffer, t_1, bound);

            OrderedFactorListNode* currentLeftOrderFactorListNode = leftOrderFactorList->head;
            //OrderedFactorListNode* currentRightOrderFactorListNode = rightOrderFactorList->head;

            while (currentLeftOrderFactorListNode != NULL){
                gmp_printf("%Zd ^ %Zd\n", currentLeftOrderFactorListNode->factor->base, currentLeftOrderFactorListNode->factor->exponent);
                currentLeftOrderFactorListNode = currentLeftOrderFactorListNode->next_node;
            }


        }


    }



    mpz_mod(t_0, t_0, modulo);

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
    return output;
}

