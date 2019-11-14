//
// Created by Andrea on 14/11/2019.
//

#ifndef TEN_PROJECT_ROUTINE_TEST_H
#define TEN_PROJECT_ROUTINE_TEST_H

#include <stdlib.h>
#include <gmp.h>
#include <stdbool.h>
#include "routines.h"

void printGMPNumber(void **data) {
    fprintf(stderr, "%s\n", mpz_get_str(NULL, 10, *data));
}

void computeFactorBase_test() {

//#define computeFactorBase_audit if(1)
    mpz_t smoothnessBound;

    if (mpz_init_set_str(smoothnessBound, "89", 10) != 0)
        exit(EXIT_FAILURE);

    FactorBase* factorBase = computeFactorBase(smoothnessBound);
    performAction(factorBase, &printGMPNumber);

    mpz_clear(smoothnessBound);
}

bool isBSmooth(mpz_t input, mpz_t smoothnessBound) {

    FactorList* factorList;

    if (factorList->length != 0 && mpz_cmp(factorList->tail->data, smoothnessBound) <= 0)
        return true;
    else
        return false;
}


#endif //TEN_PROJECT_ROUTINE_TEST_H
