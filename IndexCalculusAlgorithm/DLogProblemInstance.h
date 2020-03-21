#pragma once

#include <gmp.h>

#include "DiscreteLogarithm.h"
#include "FactorBase.h"
#include "UserInput.h"

#include "../Math/RandomNumber.h"
#include "../DataStructure/SemiLockFreeQueue.h"

typedef struct {

    void *dLogProblemInstance;

    SemiLockFreeQueue* semiLockFreeQueue;
    bool stoppingCondition;

} ThreadsPoolData;

typedef struct {

    FactorBase *factorBase;
    DiscreteLogarithm *discreteLogarithm;
    NumbersBuffer *numbersBuffer;
    RandomIntegerGenerator *randomIntegerGenerator;
    ThreadsPoolData *threadsPoolData;

    __mpz_struct *smoothnessBound;
    __mpz_struct *smoothnessBoundSqrt;
    __mpz_struct **solutionOfSecondPhase;

    unsigned int threadsPoolSize;
    unsigned int currentIndexCalculusAlgorithmStep;

} DLogProblemInstance;

DLogProblemInstance *allocateDLogProblemInstance(DLogProblemInstanceInput *input, unsigned int threadsPoolSize);

void stopThreadsPool(DLogProblemInstance *instance);

void freeDLogProblemInstance(DLogProblemInstance *input);

void freeThreadsPoolData(ThreadsPoolData *input);