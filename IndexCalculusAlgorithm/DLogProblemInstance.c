#include <pthread.h>
#include "DLogProblemInstance.h"
#include "../Math/Number.h"

ThreadsPoolData *allocateThreadsPoolData(DLogProblemInstance *dLogProblemInstance) {

    ThreadsPoolData *output = malloc(sizeof(ThreadsPoolData));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {

        output->semiLockFreeQueue = allocateSemiLockFreeQueue();
        output->stoppingCondition = false;
        output->dLogProblemInstance = dLogProblemInstance;
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
        output->threadsPoolData = allocateThreadsPoolData(output);

        output->smoothnessBound = input->smoothnessBound;
        output->randomIntegerGenerator = input->randomIntegerGenerator;
        output->numbersBuffer = input->numbersBuffer;
        output->threadsPoolSize = threadsPoolSize;

        output->smoothnessBoundSqrt = allocateNumber();
        mpz_sqrt(output->smoothnessBoundSqrt, output->smoothnessBound);
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
    freeNumber(input->smoothnessBoundSqrt);

    free(input);
}

void freeThreadsPoolData(ThreadsPoolData *input) {
//    free(input->arrayOfCircularBuffer);
    free(input);
}

void stopThreadsPool(DLogProblemInstance *instance) {
    instance->threadsPoolData->stoppingCondition = true;
}