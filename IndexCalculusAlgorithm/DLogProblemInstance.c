#include <pthread.h>
#include "DLogProblemInstance.h"
#include "../Math/Number.h"

ThreadsPoolData *allocateThreadsPoolData(DLogProblemInstance *dLogProblemInstance) {

    ThreadsPoolData *output = malloc(sizeof(ThreadsPoolData));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->sharedBuffer = allocateCircularBuffer();
        output->stoppingCondition = false;
        output->dLogProblemInstance = dLogProblemInstance;

        if (pthread_mutex_init(&(output->pthreadMutex), NULL) != 0)
            exit(EXIT_FAILURE);

        if (pthread_cond_init(&(output->pthreadCondition), NULL) != 0)
            exit(EXIT_FAILURE);
    }

    return output;
}

DLogProblemInstance *allocateDLogProblemInstance(DLogProblemInstanceInput *input) {

    DLogProblemInstance *output = malloc(sizeof(DLogProblemInstance));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {

        output->discreteLogarithm = allocateDiscreteLogarithm(input->dLogBase, input->dLogArgument, input->multiplicativeGroup);
        output->threadsPoolData = allocateThreadsPoolData(output);

        output->smoothnessBound = input->smoothnessBound;
        output->maxRandomInteger = input->maxRandomInteger;
        output->randomIntegerGenerator = input->randomIntegerGenerator;
        output->numbersBuffer = input->numbersBuffer;
    }

    return output;
}

void freeDLogProblemInstance(DLogProblemInstance *input) {

    freeNumbersBuffer(input->numbersBuffer);
    freeRandomIntegerGenerator(input->randomIntegerGenerator);
    freeDiscreteLogarithm(input->discreteLogarithm);
    freeFactorBase(input->factorBase);
    freeThreadsPoolData(input->threadsPoolData);

    freeNumber(input->maxRandomInteger);
    freeNumber(input->smoothnessBound);
}

void freeThreadsPoolData(ThreadsPoolData *input) {
    deallocateCircularBuffer(input->sharedBuffer);
}

void stopThreadsPool(DLogProblemInstance *instance) {
    instance->threadsPoolData->stoppingCondition = true;
}