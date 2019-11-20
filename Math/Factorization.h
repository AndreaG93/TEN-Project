#pragma once

#include "../ApplicationBuffer/ApplicationBuffer.h"
#include "OrderedFactorList.h"

OrderedFactorList *factorizeCheckingBSmoothness(ApplicationBuffer *applicationBuffer, __mpz_struct *number,
                                                __mpz_struct *smoothnessBound);