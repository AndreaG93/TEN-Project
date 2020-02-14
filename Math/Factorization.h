#pragma once

#include "OrderedFactorList.h"
#include "../Buffers/NumbersBuffer.h"
#include "RandomNumber.h"


OrderedFactorList *
factorizeOptimizedCheckingBSmoothness(__mpz_struct *input, __mpz_struct *modulo, __mpz_struct *smoothnessBound,
                                      NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator);


OrderedFactorList *factorizeCheckingBSmoothness(__mpz_struct *input, __mpz_struct *smoothnessBound, int maxPollardRhoTrials, NumbersBuffer *numbersBuffer);

OrderedFactorList *factorize(__mpz_struct *input, NumbersBuffer *numbersBuffer);