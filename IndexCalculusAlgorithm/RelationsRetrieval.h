#pragma once

#include "../Math/OrderedFactorList.h"

__mpz_struct **getRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator);

void *threadRoutineForRelationRetrieval(void *input);

