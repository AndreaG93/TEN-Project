#pragma once
#include <gmp.h>
#include "DiscreteLogarithm.h"
#include "../Math/RandomNumber.h"
#include "FactorBase.h"

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
    DiscreteLogarithm *discreteLogarithmToCompute;

    ApplicationBuffer* applicationBuffer;

    __mpz_struct **indexMethodSecondPhaseOutput;
    __mpz_struct *maxRandomInteger;

    SecondPhaseOutput* secondPhaseOutput;

} DLogProblemInstance;

DLogProblemInstance* allocateDLogProblemInstance(char *moduloOfMultiplicativeGroup, char *dLogBase, char *dLogArgument, unsigned long long maxRandomInteger);

void setSmoothnessBound(DLogProblemInstance *instance, char *smoothnessBoundAsString);
