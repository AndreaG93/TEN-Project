#pragma once

#include <gmp.h>

typedef struct {
    __mpz_struct* base;
    __mpz_struct* exponent;
} Factor;

Factor *allocateFactor();