#pragma once

#include <gmp.h>
#include "DiscreteLogarithm.h"
#include "../ApplicationBuffer/ApplicationBuffer.h"
#include "../Math/RandomNumber.h"
#include "SecondPhase.h"

typedef struct {

    __mpz_struct* primitiveRoot;
    __mpz_struct* moduloOfMultiplicativeGroup;
    __mpz_struct* moduloOfMultiplicativeGroupMinusOne;
    __mpz_struct* smoothnessBound;
    __mpz_struct** factorBase;
    unsigned long long factorBaseLength;

    DiscreteLogarithm *discreteLogarithmToCompute;

    ApplicationBuffer* applicationBuffer;

    __mpz_struct **indexMethodSecondPhaseOutput;

    RandomIntegerGenerator* randomIntegerGenerator;
    __mpz_struct *maxRandomInteger;

    SecondPhaseOutput* secondPhaseOutput;

} DLogProblemInstance;

DLogProblemInstance* allocateDLogProblemInstance(char *moduloOfMultiplicativeGroup, char *dLogBase, char *dLogArgument);