#include <stdlib.h>
#include "Factor.h"

Factor *allocateFactor(__mpz_struct *base, __mpz_struct *exponent) {

    Factor *output = malloc(sizeof(Factor));

    if (output == NULL) {
        exit(EXIT_FAILURE);
    } else {

        output->base = base;
        output->exponent = exponent;

        return output;
    }
}