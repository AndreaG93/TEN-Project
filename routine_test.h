//
// Created by Andrea on 14/11/2019.
//

#ifndef TEN_PROJECT_ROUTINE_TEST_H
#define TEN_PROJECT_ROUTINE_TEST_H

#include <stdlib.h>
#include <gmp.h>
#include <stdbool.h>
#include "routines.h"
#include "GMPBuffer.h"


void getMultiplicativeInverseWithExtendedEuclideanAlgorithm_test(){

    mpz_ptr* GMPBuffer = initGMPBuffer();
    mpz_ptr input = getNumberFromGMPBuffer(GMPBuffer, 1);
    mpz_ptr modulo = getNumberFromGMPBuffer(GMPBuffer, 9);

    mpz_set_si(input, 13);
    mpz_set_si(modulo, 1);

    mpz_sub(input, modulo, input);
    fprintf(stderr, "%s\n", mpz_get_str(NULL, 10, input));

    /*
    if (mpz_init_set_str(input, "5", 10) != 0)
        exit(EXIT_FAILURE);

    if (mpz_init_set_str(modulo, "178", 10) != 0)
        exit(EXIT_FAILURE);

    inverse = getMultiplicativeInverseWithExtendedEuclideanAlgorithm(GMPBuffer, input, modulo);
    fprintf(stderr, "%s\n", mpz_get_str(NULL, 10, *inverse));
     */
}













void printGMPNumber(void **data) {
    fprintf(stderr, "%s\n", mpz_get_str(NULL, 10, *data));
}

/*
void computeFactorBase_test() {

//#define computeFactorBase_audit if(1)
    mpz_t smoothnessBound;

    if (mpz_init_set_str(smoothnessBound, "89", 10) != 0)
        exit(EXIT_FAILURE);

    FactorBase* factorBase = computeFactorBase(smoothnessBound);
    performAction(factorBase, &printGMPNumber);

    mpz_clear(smoothnessBound);
}
*/
bool isBSmooth(mpz_t input, mpz_t smoothnessBound) {

    FactorList* factorList;

    if (factorList->length != 0 && mpz_cmp(factorList->tail->data, smoothnessBound) <= 0)
        return true;
    else
        return false;
}


#endif //TEN_PROJECT_ROUTINE_TEST_H
