//
// Created by andrea on 20/11/19.
//

#include <gmp.h>
#include <stdbool.h>
#include "Common.h"
#include "Number.h"
#include "RandomNumber.h"
#include "OrderedFactorList.h"
#include "Factorization.h"
#include "../Error.h"


#define DEFINITELY_NOT_PRIME 0

bool isPrime(__mpz_struct *input) {

    switch (mpz_probab_prime_p(input, 1)) {
        case DEFINITELY_NOT_PRIME:
            return false;
        default:
            return true;
    }
}

bool isInvertible(NumbersBuffer *numbersBuffer, mpz_t input, mpz_t modulo) {

    __mpz_struct *gcd = retrieveNumberFromBuffer(numbersBuffer);

    mpz_gcd(gcd, input, modulo);
    bool output = (mpz_cmp_ui(gcd, 1) == 0);

    releaseNumber(numbersBuffer);

    return output;
}