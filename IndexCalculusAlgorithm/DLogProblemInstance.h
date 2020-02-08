#pragma once
#include <gmp.h>
#include "DiscreteLogarithm.h"
#include "../Math/RandomNumber.h"
#include "FactorBase.h"
#include "../Buffers/CircularBuffer.h"
#include "../UserInput.h"

typedef struct {

    __mpz_struct **solution;
    __mpz_struct *base;

} SecondPhaseOutput;

typedef struct {

    void *dLogProblemInstance;

    CircularBuffer *sharedBuffer;
    bool stoppingCondition;
    bool pauseCondition;

    pthread_cond_t pthreadCondition;
    pthread_mutex_t pthreadMutex;

} ThreadsPoolData;

typedef struct {

    unsigned char currentIndexCalculusAlgorithmStep;

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

DLogProblemInstance *allocateDLogProblemInstance(DLogProblemInstanceInput *input);

void sendSignalToThreadsPoolToExecuteSpecifiedAlgorithmStep(DLogProblemInstance *instance, unsigned char algorithmStep);

void pauseThreadsPool(DLogProblemInstance *instance);