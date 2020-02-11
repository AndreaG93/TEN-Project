#pragma once

#include <gmp.h>

typedef struct {

    __gmp_randstate_struct state;
    __mpz_struct *maxRandomInteger;

} RandomIntegerGenerator;


RandomIntegerGenerator *allocateRandomIntegerGenerator(__mpz_struct *maxRandomInteger);

__mpz_struct* selectUniformlyDistributedRandomInteger(RandomIntegerGenerator *input);

void freeRandomIntegerGenerator(RandomIntegerGenerator *input);