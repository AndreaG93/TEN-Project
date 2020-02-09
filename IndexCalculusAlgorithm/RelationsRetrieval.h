#pragma once

void *threadRoutineForRelationRetrieval(void *input);

__mpz_struct **getLogarithmRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator, __mpz_struct* logarithmArgument);