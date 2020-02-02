#pragma once

#include "OrderedFactorList.h"
#include "../Buffers/NumbersBuffer.h"
#include "RandomNumber.h"

OrderedFactorList *factorize(__mpz_struct *input, __mpz_struct *modulo, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator);