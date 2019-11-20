#pragma once

#include <gmp.h>
#include <stdbool.h>
#include <string.h>
#include "DataStructures/UnorderedList.h"
#include "../Math/Number.h"

void PollardSRhoFunction(mpz_t x, mpz_t modulo) {
    mpz_mul(x, x, x);
    mpz_add_ui(x, x, 2);
    mpz_mod(x, x, modulo);
}

bool isInvertible(mpz_t input, mpz_t modulo, __mpz_struct **appBuffer) {
/*
    __mpz_struct *gcd = getNumberFromApplicationBuffer(appBuffer, 0);

    mpz_gcd(gcd, input, modulo);
    return (mpz_cmp_ui(gcd, 1) == 0);
    */
    return true;
}

bool isBSmooth(mpz_t input, mpz_t smoothnessBound) {
/*
    FactorList* factorList;

    if (factorList->length != 0 && mpz_cmp(factorList->tail->data, smoothnessBound) <= 0)
        return true;
    else
        return false;
        */
    return false;
}

mpz_ptr getInverseMultiplicative(__mpz_struct *input, __mpz_struct *modulo, __mpz_struct **appBuffer) {

    /*
    __mpz_struct *y_0 = malloc(sizeof(mpz_ptr));
    if (y_0 == NULL)
        exit(EXIT_FAILURE);
    else
        mpz_init_set_si(y_0, 0);

    __mpz_struct *x_0 = getNumberFromApplicationBuffer(appBuffer, 0);
    __mpz_struct *x_1 = getNumberFromApplicationBuffer(appBuffer, 1);
    __mpz_struct *y_1 = getNumberFromApplicationBuffer(appBuffer, 2);
    __mpz_struct *quotient = getNumberFromApplicationBuffer(appBuffer, 3);
    __mpz_struct *remainder_0 = getNumberFromApplicationBuffer(appBuffer, 4);
    __mpz_struct *remainder_1 = getNumberFromApplicationBuffer(appBuffer, 5);
    __mpz_struct *aux_1 = getNumberFromApplicationBuffer(appBuffer, 6);
    __mpz_struct *aux_2 = getNumberFromApplicationBuffer(appBuffer, 7);
    __mpz_struct *aux_3 = getNumberFromApplicationBuffer(appBuffer, 8);
    __mpz_struct *aux_4 = getNumberFromApplicationBuffer(appBuffer, 9);

    mpz_set_si(x_0, 1);
    mpz_set_si(x_1, 0);
    mpz_set_si(y_1, 1);
    mpz_set(remainder_0, modulo);
    mpz_set(remainder_1, input);

    while (mpz_cmp_ui(remainder_1, 0) != 0) {
        mpz_tdiv_q(quotient, remainder_0, remainder_1);

        mpz_mul(aux_1, x_1, quotient);
        mpz_mul(aux_2, y_1, quotient);

        mpz_set(aux_3, x_1);
        mpz_set(aux_4, y_1);

        mpz_sub(x_1, x_0, aux_1);
        mpz_sub(y_1, y_0, aux_2);

        mpz_set(x_0, aux_3);
        mpz_set(y_0, aux_4);

        mpz_mul(aux_1, x_1, modulo);
        mpz_mul(aux_2, y_1, input);

        mpz_set(remainder_0, remainder_1);
        mpz_add(remainder_1, aux_1, aux_2);
    }

    return y_0;
     */
}

void generatePrimeDatabase() {

    char *currentPrimeStr;
    size_t numOfDigits;

    FILE *outputFile = fopen("/Andrea/PrimeDatabase.txt", "w+");
    if (outputFile == NULL)
        exit(EXIT_FAILURE);

    __mpz_struct *currentPrime = allocateNumber();

    mpz_set_ui(currentPrime, 999999999989);

    while (true) {
        mpz_nextprime(currentPrime, currentPrime);

        currentPrimeStr = mpz_get_str(NULL, 10, currentPrime);
        numOfDigits = strlen(currentPrimeStr);
        fprintf(outputFile, "%s\n", currentPrimeStr);

        if (numOfDigits == 15) {
            break;
        }
    }

    if (fclose(outputFile) != 0) {
        exit(EXIT_FAILURE);
    }
}