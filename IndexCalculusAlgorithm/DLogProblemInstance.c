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
        output->pauseCondition = true;
        output->dLogProblemInstance = dLogProblemInstance;

        if (pthread_mutex_init(&(output->pthreadMutex), NULL) != 0)
            exit(EXIT_FAILURE);

        if (pthread_cond_init(&(output->pthreadCondition), NULL) != 0)
            exit(EXIT_FAILURE);
    }

    return output;
}

void setModuloOfMultiplicativeGroup(DLogProblemInstance *instance, char *moduloOfMultiplicativeGroup) {
    instance->moduloOfMultiplicativeGroup = allocateAndSetNumberFromString(moduloOfMultiplicativeGroup);
    instance->moduloOfMultiplicativeGroupMinusOne = allocateNumber();
    mpz_sub_ui(instance->moduloOfMultiplicativeGroupMinusOne, instance->moduloOfMultiplicativeGroup, 1);
}

DLogProblemInstance *allocateAndInitializeDLogProblemInstance(char *moduloOfMultiplicativeGroup, char *dLogBase, char *dLogArgument, char *smoothnessBound, unsigned long long maxRandomInteger, size_t numbersBufferSize) {

    DLogProblemInstance *output = malloc(sizeof(DLogProblemInstance));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        setModuloOfMultiplicativeGroup(output, moduloOfMultiplicativeGroup);

        output->discreteLogarithm = allocateDiscreteLogarithm(dLogBase, dLogArgument, output->moduloOfMultiplicativeGroup);
        output->smoothnessBound = allocateAndSetNumberFromString(smoothnessBound);
        output->maxRandomInteger = allocateAndSetNumberFromULL(maxRandomInteger);
        output->numbersBufferSize = numbersBufferSize;
        output->numbersBuffer = allocateNumbersBuffer(output->numbersBufferSize);
        output->threadsPoolData = allocateThreadsPoolData(output);
    }

    return output;
}

void sendSignalToThreadsPoolToExecuteSpecifiedAlgorithmStep(DLogProblemInstance *instance, unsigned char algorithmStep) {

    instance->currentIndexCalculusAlgorithmStep = algorithmStep;
    instance->threadsPoolData->pauseCondition = false;

    pthread_cond_signal(&instance->threadsPoolData->pthreadCondition);
}

void pauseThreadsPool(DLogProblemInstance *instance) {
    instance->threadsPoolData->pauseCondition = true;
}