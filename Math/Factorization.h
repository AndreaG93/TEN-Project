#pragma once

#include "OrderedFactorList.h"
#include "../Buffers/NumbersBuffer.h"
#include "RandomNumber.h"

OrderedFactorList *factorizeCheckingBSmoothness(__mpz_struct *input, __mpz_struct *smoothnessBound, int maxPollardRhoTrials, NumbersBuffer *numbersBuffer);

OrderedFactorList *factorize(__mpz_struct *input, NumbersBuffer *numbersBuffer);

OrderedFactorList *factorizeCheckingBSmoothnessOptimized(__mpz_struct* input, __mpz_struct* multiplicativeGroup, __mpz_struct* magnitudeOfMultiplicativeGroup, __mpz_struct* smoothnessBound, NumbersBuffer *numbersBuffer, mpz_t sqrtB);