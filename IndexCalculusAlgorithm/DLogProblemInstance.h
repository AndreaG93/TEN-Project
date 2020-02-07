#pragma once
#include <gmp.h>
#include "DiscreteLogarithm.h"
#include "../Math/RandomNumber.h"
#include "FactorBase.h"
#include "../Buffers/CircularBuffer.h"

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

    __mpz_struct *primitiveRoot;
    __mpz_struct *moduloOfMultiplicativeGroup;
    __mpz_struct *moduloOfMultiplicativeGroupMinusOne;
    __mpz_struct *smoothnessBound;

    FactorBase *factorBase;
    DiscreteLogarithm *discreteLogarithm;
    ThreadsPoolData *threadsPoolData;
    NumbersBuffer *numbersBuffer;

    __mpz_struct **indexMethodSecondPhaseOutput;

    SecondPhaseOutput *secondPhaseOutput;


    size_t numbersBufferSize;
    __mpz_struct *maxRandomInteger;

} DLogProblemInstance;

DLogProblemInstance *allocateAndInitializeDLogProblemInstance(char *moduloOfMultiplicativeGroup, char *dLogBase, char *dLogArgument, char *smoothnessBound, unsigned long long maxRandomInteger, size_t numbersBufferSize);

void sendSignalToThreadsPoolToExecuteSpecifiedAlgorithmStep(DLogProblemInstance *instance, unsigned char algorithmStep);

void pauseThreadsPool(DLogProblemInstance *instance);