#pragma once

#include <gmp.h>

typedef struct {

    __gmp_randstate_struct state;
    __mpz_struct *maxRandomInteger;

} RandomIntegerGenerator;


RandomIntegerGenerator *allocateRandomIntegerGenerator(__mpz_struct *maxRandomInteger);

void selectUniformlyDistributedRandomInteger(RandomIntegerGenerator *input, __mpz_struct *output);

void deallocateRandomIntegerGenerator(RandomIntegerGenerator *input);