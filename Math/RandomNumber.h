#pragma once

#include <gmp.h>

typedef __gmp_randstate_struct RandomIntegerGenerator;

RandomIntegerGenerator* initializeRandomIntegerGenerator();

__mpz_struct* getUniformlyDistributedRandomInteger(RandomIntegerGenerator* generator, __mpz_struct* max);