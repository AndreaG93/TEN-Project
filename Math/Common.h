#pragma once


#include "../Buffers/NumbersBuffer.h"

bool isInvertible(NumbersBuffer *applicationBuffer, mpz_t input, mpz_t modulo);

__mpz_struct *getInverseMultiplicative(NumbersBuffer *applicationBuffer, __mpz_struct *input, __mpz_struct *modulo);