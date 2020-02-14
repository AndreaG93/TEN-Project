#pragma once

#include <gmp.h>
#include "DiscreteLogarithm.h"
#include "../Math/RandomNumber.h"
#include "FactorBase.h"
#include "../Buffers/CircularBuffer.h"
#include "UserInput.h"

typedef struct {
    __mpz_struct **solution;
} SecondPhaseOutput;

typedef struct {

    void *dLogProblemInstance;

    CircularBuffer **arrayOfCircularBuffer;
    bool stoppingCondition;

    pthread_cond_t pthreadCondition;
    pthread_mutex_t pthreadMutex;

} ThreadsPoolData;

typedef struct {

    FactorBase *factorBase;
    DiscreteLogarithm *discreteLogarithm;
    NumbersBuffer *numbersBuffer;
    RandomIntegerGenerator *randomIntegerGenerator;
    ThreadsPoolData *threadsPoolData;

    __mpz_struct *smoothnessBound;
    __mpz_struct **solutionOfSecondPhase;

    unsigned int threadsPoolSize;
    unsigned int currentIndexCalculusAlgorithmStep;
    unsigned int currentThreadIDFromWhichExtractData;

} DLogProblemInstance;

DLogProblemInstance *allocateDLogProblemInstance(DLogProblemInstanceInput *input, unsigned int threadsPoolSize);

void stopThreadsPool(DLogProblemInstance *instance);

void freeDLogProblemInstance(DLogProblemInstance *input);

void freeThreadsPoolData(ThreadsPoolData *input);

void *popFromArrayOfCircularBufferRoundRobinManner(DLogProblemInstance* instance);