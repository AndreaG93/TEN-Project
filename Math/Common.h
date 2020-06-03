#pragma once


#include "../Buffers/NumbersBuffer.h"
#include "RandomNumber.h"

bool isInvertible(NumbersBuffer *numbersBuffer, mpz_t input, mpz_t modulo);

bool isPrime(__mpz_struct *input);