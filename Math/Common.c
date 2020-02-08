//
// Created by andrea on 20/11/19.
//

#include <gmp.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Common.h"
#include "Number.h"

bool isInvertible(NumbersBuffer *numbersBuffer, mpz_t input, mpz_t modulo) {

    __mpz_struct *gcd = retrieveNumberFromBuffer(numbersBuffer);

    mpz_gcd(gcd, input, modulo);
    bool output = (mpz_cmp_ui(gcd, 1) == 0);

    releaseNumber(numbersBuffer);

    return output;
}

__mpz_struct *getInverseMultiplicative(__mpz_struct *input, __mpz_struct *modulo, NumbersBuffer *numbersBuffer) {

    __mpz_struct *output = allocateNumber();

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 11);

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
    }

    releaseNumbers(numbersBuffer, 11);

    mpz_mod(output, t_0, modulo);
    return output;
}