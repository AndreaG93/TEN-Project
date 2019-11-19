#pragma once

#include "../ApplicationBuffer/ApplicationBuffer.h"
#include "OrderedFactorList.h"

OrderedFactorList *factorize(ApplicationBuffer *applicationBuffer, __mpz_struct *number);