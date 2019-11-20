#pragma once

#include "../ApplicationBuffer/ApplicationBuffer.h"

bool isInvertible(ApplicationBuffer *applicationBuffer, mpz_t input, mpz_t modulo);

__mpz_struct *getInverseMultiplicative(ApplicationBuffer *applicationBuffer, __mpz_struct *input, __mpz_struct *modulo);