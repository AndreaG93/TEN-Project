#pragma once

#include "OrderedFactorList.h"
#include "../Buffers/NumbersBuffer.h"
#include "RandomNumber.h"

OrderedFactorList *factorizeCheckingBSmoothness(__mpz_struct *input, __mpz_struct *smoothnessBound, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator);

OrderedFactorList *factorizeOptimized(__mpz_struct *input, __mpz_struct *modulo, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator);