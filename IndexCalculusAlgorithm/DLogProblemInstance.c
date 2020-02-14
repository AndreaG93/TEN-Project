#include <pthread.h>
#include "DLogProblemInstance.h"
#include "../Math/Number.h"

ThreadsPoolData *allocateThreadsPoolData(DLogProblemInstance *dLogProblemInstance, unsigned int threadsPoolSize) {

    ThreadsPoolData *output = malloc(sizeof(ThreadsPoolData));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->arrayOfCircularBuffer = malloc(threadsPoolSize * sizeof(CircularBuffer*));
        if (output->arrayOfCircularBuffer == NULL)
            exit(EXIT_FAILURE);

        for (unsigned int index = 0; index < threadsPoolSize; index++)
            output->arrayOfCircularBuffer[index] = allocateCircularBuffer();

        output->stoppingCondition = false;
        output->dLogProblemInstance = dLogProblemInstance;

        if (pthread_mutex_init(&(output->pthreadMutex), NULL) != 0)
            exit(EXIT_FAILURE);

        if (pthread_cond_init(&(output->pthreadCondition), NULL) != 0)
            exit(EXIT_FAILURE);
    }

    return output;
}

DLogProblemInstance *allocateDLogProblemInstance(DLogProblemInstanceInput *input, unsigned int threadsPoolSize) {

    DLogProblemInstance *output = malloc(sizeof(DLogProblemInstance));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {

        output->currentIndexCalculusAlgorithmStep = 2;

        output->discreteLogarithm = allocateDiscreteLogarithm(input->dLogBase, input->dLogArgument, input->multiplicativeGroup);
        output->threadsPoolData = allocateThreadsPoolData(output, threadsPoolSize);

        output->smoothnessBound = input->smoothnessBound;
        output->randomIntegerGenerator = input->randomIntegerGenerator;
        output->numbersBuffer = input->numbersBuffer;

        output->threadsPoolSize = threadsPoolSize;
        output->currentThreadIDFromWhichExtractData = 0;
    }

    return output;
}

void freeDLogProblemInstance(DLogProblemInstance *input) {

    freeNumbersArray(input->solutionOfSecondPhase, input->factorBase->length);
    freeNumbersBuffer(input->numbersBuffer);
    freeRandomIntegerGenerator(input->randomIntegerGenerator);
    freeDiscreteLogarithm(input->discreteLogarithm);
    freeFactorBase(input->factorBase);
    freeThreadsPoolData(input->threadsPoolData);
    freeNumber(input->smoothnessBound);

    free(input);
}

void freeThreadsPoolData(ThreadsPoolData *input) {
    free(input->arrayOfCircularBuffer);
    free(input);
}

void stopThreadsPool(DLogProblemInstance *instance) {
    instance->threadsPoolData->stoppingCondition = true;
}

void *popFromArrayOfCircularBufferRoundRobinManner(DLogProblemInstance* instance) {

    CircularBuffer **buffers = instance->threadsPoolData->arrayOfCircularBuffer;

    void *output = NULL;

    while (output == NULL) {
        output = popFromCircularBuffer(buffers[instance->currentThreadIDFromWhichExtractData]);
        instance->currentThreadIDFromWhichExtractData = (instance->currentThreadIDFromWhichExtractData + 1) % instance->threadsPoolSize;
    }

    return output;
}