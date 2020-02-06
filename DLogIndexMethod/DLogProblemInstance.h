#pragma once
#include <gmp.h>
#include "DiscreteLogarithm.h"
#include "../Math/RandomNumber.h"
#include "FactorBase.h"
#include "../ThreadsPool/ThreadsPool.h"

typedef struct {

    __mpz_struct** solution;
    __mpz_struct* base;

} SecondPhaseOutput;

typedef struct {

    __mpz_struct* primitiveRoot;
    __mpz_struct* moduloOfMultiplicativeGroup;
    __mpz_struct* moduloOfMultiplicativeGroupMinusOne;
    __mpz_struct* smoothnessBound;

    FactorBase* factorBase;
    DiscreteLogarithm *discreteLogarithm;

    NumbersBuffer* numbersBuffer;

    __mpz_struct **indexMethodSecondPhaseOutput;

    SecondPhaseOutput* secondPhaseOutput;

    unsigned char currentPhase;

    ThreadsPoolData *threadsPoolData;
    RandomIntegerGenerator *randomIntegerGenerator;

} DLogProblemInstance;

DLogProblemInstance* allocateDLogProblemInstance();