#pragma once

#include <gmp.h>

typedef __gmp_randstate_struct RandomIntegerGenerator;

RandomIntegerGenerator* initializeRandomNumberGenerator();

__mpz_struct* getUniformlyDistributedRandomInteger(RandomIntegerGenerator* generator, __mpz_struct* max);