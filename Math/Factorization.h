#pragma once

#include "OrderedFactorList.h"
#include "../Buffers/NumbersBuffer.h"
#include "RandomNumber.h"

OrderedFactorList *
factorizeCheckingBSmoothness(__mpz_struct *input, __mpz_struct *smoothnessBound, NumbersBuffer *numbersBuffer,
                             RandomIntegerGenerator *randomIntegerGenerator);

OrderedFactorList *
factorizeOptimizedCheckingBSmoothness(__mpz_struct *input, __mpz_struct *modulo, __mpz_struct *smoothnessBound,
                                      NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator);

__mpz_struct *getFactorUsingPollardRho(__mpz_struct *numberToFactorize, NumbersBuffer *numbersBuffer,
                                       RandomIntegerGenerator *randomIntegerGenerator);