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
    unsigned int indexOfBaseRespectToFactorBase;

} SecondPhaseOutput;

typedef struct {

    void *dLogProblemInstance;

    CircularBuffer *sharedBuffer;
    bool stoppingCondition;

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

void stopThreadsPool(DLogProblemInstance *instance);