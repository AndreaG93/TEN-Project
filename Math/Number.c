#include <gmp.h>
#include <stdlib.h>
#include "Number.h"

__mpz_struct *allocateNumber() {
    __mpz_struct *output = malloc(sizeof(__mpz_struct));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        mpz_init(output);
        return output;
    }
}

__mpz_struct *allocateAndSetULLNumber(unsigned long long number) {

    __mpz_struct *output = malloc(sizeof(__mpz_struct));
    if (output == NULL)
        exit(EXIT_FAILURE);

    mpz_init_set_ui(output, number);
    return output;
}

__mpz_struct *allocateAndSetNumber(__mpz_struct *number) {

    __mpz_struct *output = malloc(sizeof(__mpz_struct));
    if (output == NULL)
        exit(EXIT_FAILURE);

    mpz_init_set(output, number);
    return output;
}