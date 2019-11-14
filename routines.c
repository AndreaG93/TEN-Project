//
// Created by Andrea on 14/11/2019.
//

#include <gmp.h>
#include "routines.h"
#include "DataStructures/UnorderedList.h"

void copyGMPNumberIntoUnorderedList(UnorderedList *list, mpz_ptr input) {

    mpz_ptr data = malloc(sizeof(mpz_ptr));
    if (data == NULL)
        exit(EXIT_FAILURE);
    else {

        mpz_set(data, input);
        insert(list, data);
    }
}

FactorBase *computeFactorBase(mpz_ptr smoothnessBound) {

    FactorBase *output = allocUnorderedList();
    mpz_t currentPrime;

    if (mpz_cmp_ui(smoothnessBound, 1) <= 0){
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