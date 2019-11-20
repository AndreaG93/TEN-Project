#pragma once

#include <gmp.h>

typedef __gmp_randstate_struct RandomIntegerGenerator;

RandomIntegerGenerator* initializeRandomIntegerGenerator();

void selectUniformlyDistributedRandomInteger(RandomIntegerGenerator* generator, __mpz_struct* max, __mpz_struct* output);