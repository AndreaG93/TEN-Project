#pragma once

#include "OrderedFactorList.h"
#include "../Buffers/ApplicationBuffer.h"

OrderedFactorList *factorizeCheckingBSmoothness(ApplicationBuffer *applicationBuffer, __mpz_struct *number,
                                                __mpz_struct *smoothnessBound);

void factorize(ApplicationBuffer *applicationBuffer, __mpz_struct *number, __mpz_struct *modulo);