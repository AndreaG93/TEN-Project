#pragma once

#include <gmp.h>

typedef struct {

    __mpz_struct** solution;
    __mpz_struct* base;

} SecondPhaseOutput;

SecondPhaseOutput* allocateSecondPhaseOutput(unsigned long long size);

