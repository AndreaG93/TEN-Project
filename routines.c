//
// Created by Andrea on 14/11/2019.
//

#include <gmp.h>
#include <stdbool.h>
#include "routines.h"
#include "DataStructures/UnorderedList.h"
#include "GMPBuffer.h"


void copyGMPNumberIntoUnorderedList(UnorderedList *list, mpz_ptr input) {

    mpz_ptr data = malloc(sizeof(mpz_ptr));
    if (data == NULL)
        exit(EXIT_FAILURE);
    else {

        mpz_set(data, input);
        insert(list, data);
    }
}

bool isInvertible(mpz_t input, mpz_t modulo) {

    mpz_t gcd;
    mpz_gcd(gcd, input, modulo);

    return (mpz_cmp_ui(gcd, 1) == 0);
}


mpz_ptr A_mul_B_plus_B_mul_C() {

}


mpz_ptr getMultiplicativeInverseWithExtendedEuclideanAlgorithm(mpz_ptr* buffer, mpz_ptr input, mpz_ptr modulo) {

    mpz_ptr x_0 = malloc(sizeof(mpz_ptr));
    if (x_0 == NULL)
        exit(EXIT_FAILURE);

    mpz_ptr y_0 = getNumberFromGMPBuffer(buffer, 0);
    mpz_ptr x_1 = getNumberFromGMPBuffer(buffer, 1);
    mpz_ptr y_1 = getNumberFromGMPBuffer(buffer, 2);
    mpz_ptr quotient = getNumberFromGMPBuffer(buffer, 3);
    mpz_ptr remainder_0 = getNumberFromGMPBuffer(buffer, 4);
    mpz_ptr remainder_1 = getNumberFromGMPBuffer(buffer, 5);
    mpz_ptr aux_1 = getNumberFromGMPBuffer(buffer, 6);
    mpz_ptr aux_2 = getNumberFromGMPBuffer(buffer, 7);
    mpz_ptr aux_3 = getNumberFromGMPBuffer(buffer, 8);
    mpz_ptr aux_4 = getNumberFromGMPBuffer(buffer, 9);

    mpz_init_set_si(y_1, 1);
    mpz_init_set_si(y_0, 0);
    mpz_init_set_si(x_0, 1);
    mpz_init_set_si(x_1, 0);
    mpz_init_set(remainder_0, modulo);
    mpz_init_set(remainder_1, input);
    mpz_init_set_si(quotient,0);
    mpz_init_set_si(aux_1,0);
    mpz_init_set_si(aux_2,0);
    mpz_init_set_si(aux_3,0);
    mpz_init_set_si(aux_4,0);


    while (mpz_cmp_ui(remainder_1, 0) != 0) {
        mpz_tdiv_q(quotient, remainder_0, remainder_1);

        mpz_mul(aux_1, x_1, quotient);
        mpz_mul(aux_2, y_1, quotient);

        mpz_add_ui(aux_3, x_1,0);
        mpz_add_ui(aux_4, y_1,0);

        mpz_neg(aux_1, aux_1);
        mpz_neg(aux_2, aux_2);

        mpz_add(x_1, x_0, aux_1);
        mpz_add(y_1, y_0, aux_2);

        mpz_add_ui(x_0, aux_3,0);
        mpz_add_ui(y_0, aux_4,0);

        mpz_mul(aux_1, x_1, modulo);
        mpz_mul(aux_2, y_1, input);

        mpz_add_ui(remainder_0, remainder_1, 0);
        mpz_add(remainder_1, aux_1, aux_2);
    }

    return y_0;
}

FactorBase *computeFactorBase(mpz_ptr smoothnessBound) {

    FactorBase *output = allocUnorderedList();
    mpz_t currentPrime;

    if (mpz_cmp_ui(smoothnessBound, 1) <= 0) {
        return output;
    }

    mpz_init_set_ui(currentPrime, 2);

    do {
        copyGMPNumberIntoUnorderedList(output, currentPrime);
        mpz_nextprime(currentPrime, currentPrime);

        printf("CurrentPrime: %s\n", mpz_get_str(NULL, 10, currentPrime));

    } while (mpz_cmp(currentPrime, smoothnessBound) <= 0);

    mpz_clear(currentPrime);
    return output;
}