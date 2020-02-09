#pragma once


#include "../Buffers/NumbersBuffer.h"
#include "RandomNumber.h"

bool isInvertible(NumbersBuffer *applicationBuffer, mpz_t input, mpz_t modulo);

__mpz_struct *getInverseMultiplicative(__mpz_struct *input, __mpz_struct *modulo, NumbersBuffer *numbersBuffer);

bool isPrime(__mpz_struct *input);

bool isGroupGenerator(__mpz_struct *input, __mpz_struct *multiplicativeGroup, NumbersBuffer *numbersBuffer,
                      RandomIntegerGenerator *randomIntegerGenerator);