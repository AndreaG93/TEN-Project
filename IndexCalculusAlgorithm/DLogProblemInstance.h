#pragma once

#include <gmp.h>
#include "DiscreteLogarithm.h"
#include "../Math/RandomNumber.h"
#include "FactorBase.h"
#include "../Buffers/CircularBuffer.h"
#include "UserInput.h"

typedef struct {

    __mpz_struct **solution;
    __mpz_struct *base;
    unsigned int indexOfBaseRespectToFactorBase;

} SecondPhaseOutput;

typedef struct {

    void *dLogProblemInstance;

    CircularBuffer **arrayOfCircularBuffer;
    bool stoppingCondition;

    pthread_cond_t pthreadCondition;
    pthread_mutex_t pthreadMutex;

} ThreadsPoolData;

typedef struct {

    unsigned int currentIndexCalculusAlgorithmStep;
    unsigned int threadsPoolSize;
    unsigned int currentThreadIDFromWhichExtractData;

    DiscreteLogarithm *discreteLogarithm;
    FactorBase *factorBase;
    ThreadsPoolData *threadsPoolData;
    NumbersBuffer *numbersBuffer;
    RandomIntegerGenerator *randomIntegerGenerator;

    __mpz_struct *maxRandomInteger;
    __mpz_struct *smoothnessBound;
    __mpz_struct **indexMethodSecondPhaseOutput;

    SecondPhaseOutput *secondPhaseOutput;

} DLogProblemInstance;

DLogProblemInstance *allocateDLogProblemInstance(DLogProblemInstanceInput *input, unsigned int threadsPoolSize);

void stopThreadsPool(DLogProblemInstance *instance);

void freeDLogProblemInstance(DLogProblemInstance *input);

void freeThreadsPoolData(ThreadsPoolData *input);

void *popFromArrayOfCircularBufferRoundRobinManner(DLogProblemInstance* instance);