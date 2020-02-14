#include "DLogProblemInstance.h"
#include "../Math/OrderedFactorList.h"
#include "../Math/Factorization.h"
#include "../Math/Number.h"
#include "../ThreadsPool/ThreadsPool.h"

typedef struct {

    __mpz_struct *randomExponentOfGenerator;
    OrderedFactorList *factorListOfRandomBSmoothNumber;

} RawRelation;

RawRelation *allocateRawRelation() {

    RawRelation *output = malloc(sizeof(RawRelation));

    if (output == NULL) {
        exit(EXIT_FAILURE);
    } else {
        output->randomExponentOfGenerator = NULL;
        output->factorListOfRandomBSmoothNumber = NULL;
    }

    return output;
}

void freeRawRelation(RawRelation *input) {

    freeNumber(input->randomExponentOfGenerator);
    freeOrderedFactorList(input->factorListOfRandomBSmoothNumber);
    free(input);
}

RawRelation *getRawRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator) {

    RawRelation *output = allocateRawRelation();
    __mpz_struct *power = retrieveNumberFromBuffer(numbersBuffer);

    do {

        if (output->randomExponentOfGenerator != NULL)
            freeNumber(output->randomExponentOfGenerator);

        output->randomExponentOfGenerator = selectUniformlyDistributedRandomInteger(randomIntegerGenerator);

        mpz_powm(power, instance->discreteLogarithm->base, output->randomExponentOfGenerator, instance->discreteLogarithm->multiplicativeGroup);

        if (instance->currentIndexCalculusAlgorithmStep == 3) {
            mpz_mul(power, power, instance->discreteLogarithm->argument);
        }

        mpz_mod(power, power, instance->discreteLogarithm->multiplicativeGroup);

        output->factorListOfRandomBSmoothNumber = factorizeCheckingBSmoothnessOptimized(power, instance->discreteLogarithm->multiplicativeGroup, instance->discreteLogarithm->magnitudeOfMultiplicativeGroup, instance->smoothnessBound, numbersBuffer, instance->smoothnessBoundSqrt);

    } while (output->factorListOfRandomBSmoothNumber == NULL);

    releaseNumber(numbersBuffer);


    return output;
}

__mpz_struct **getRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator) {

    RawRelation *rawRelation = getRawRelation(instance, numbersBuffer, randomIntegerGenerator);
    __mpz_struct **output = allocateNumbersArray(instance->factorBase->length + 1, true);

    OrderedFactorListNode *currentNodeOfRawRelation = rawRelation->factorListOfRandomBSmoothNumber->head;

    int index = 0;

    if (instance->currentIndexCalculusAlgorithmStep == 2) {


        for (FactorBaseNode *currentFactorBaseNode = instance->factorBase->head;
             currentFactorBaseNode != NULL; currentFactorBaseNode = currentFactorBaseNode->next_node, index++) {

            if (currentNodeOfRawRelation != NULL && mpz_cmp(currentNodeOfRawRelation->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                mpz_set(*(output + index), currentNodeOfRawRelation->factor->exponent);
                currentNodeOfRawRelation = currentNodeOfRawRelation->next_node;

            } else
                mpz_set_ui(*(output + index), 0);
        }

        mpz_set(*(output + instance->factorBase->length), rawRelation->randomExponentOfGenerator);
    }

    if (instance->currentIndexCalculusAlgorithmStep == 3) {

        for (FactorBaseNode *currentFactorBaseNode = instance->factorBase->head;
             currentFactorBaseNode != NULL; currentFactorBaseNode = currentFactorBaseNode->next_node, index++) {

            if (currentNodeOfRawRelation != NULL && mpz_cmp(currentNodeOfRawRelation->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                mpz_set(*(output + index), currentNodeOfRawRelation->factor->exponent);
                currentNodeOfRawRelation = currentNodeOfRawRelation->next_node;

            } else
                mpz_set_ui(*(output + index), 0);
        }

        mpz_set(*(output + instance->factorBase->length), rawRelation->randomExponentOfGenerator);
        mpz_mul_si(*(output + instance->factorBase->length), *(output + instance->factorBase->length), -1);
    }

    freeRawRelation(rawRelation);
    return output;
}

void *threadRoutineForRelationRetrieval(void *input) {

    ThreadArgument *threadArgument = (ThreadArgument *) input;
    unsigned int threadID = threadArgument->threadID;
    ThreadsPoolData *threadsPoolData = (ThreadsPoolData *) threadArgument->threadArgument;
    DLogProblemInstance *instance = (DLogProblemInstance *) threadsPoolData->dLogProblemInstance;

    NumbersBuffer *numbersBuffer = allocateNumbersBuffer(instance->numbersBuffer->size);
    RandomIntegerGenerator *randomIntegerGenerator = allocateRandomIntegerGenerator(
            instance->discreteLogarithm->multiplicativeGroup);
    CircularBuffer *circularBuffer = threadsPoolData->arrayOfCircularBuffer[threadID];

    while (threadsPoolData->stoppingCondition != true) {

        __mpz_struct **relation = getRelation(instance, numbersBuffer, randomIntegerGenerator);
        pushIntoCircularBuffer(circularBuffer, relation);
    }

    free(threadArgument);
    freeCircularBuffer(circularBuffer);
    freeNumbersBuffer(numbersBuffer);
    freeRandomIntegerGenerator(randomIntegerGenerator);

    return NULL;
}