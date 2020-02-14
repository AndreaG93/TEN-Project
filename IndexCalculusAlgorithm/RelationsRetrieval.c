#include "DLogProblemInstance.h"
#include "../Math/OrderedFactorList.h"
#include "../Math/Factorization.h"
#include "../Math/Number.h"
#include "../ThreadsPool/ThreadsPool.h"
#include "RelationsRetrieval.h"

typedef struct {

    __mpz_struct *randomExponentOfGenerator;
    OrderedFactorList *factorListOfRandomBSmoothNumber;

} RawRelation;

RawRelation *allocateRelation() {

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

    RawRelation *output = allocateRelation();

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 1);

    __mpz_struct *power = buffer[0];

    __mpz_struct *sqrtB = allocateNumber(); // TODO release this number
    mpz_sqrt(sqrtB, instance->smoothnessBound);

    do {

        if (output->randomExponentOfGenerator != NULL)
            freeNumber(output->randomExponentOfGenerator);

        output->randomExponentOfGenerator = selectUniformlyDistributedRandomInteger(randomIntegerGenerator);

        mpz_powm(power, instance->discreteLogarithm->base, output->randomExponentOfGenerator, instance->discreteLogarithm->multiplicativeGroup);

        if (instance->currentIndexCalculusAlgorithmStep == 3) {
            mpz_mul(power, power, instance->discreteLogarithm->argument);
        }

        mpz_mod(power, power, instance->discreteLogarithm->multiplicativeGroup);

        output->factorListOfRandomBSmoothNumber = factorizeCheckingBSmoothness(power, instance->smoothnessBound, mpz_get_ui(sqrtB), numbersBuffer);

    } while (output->factorListOfRandomBSmoothNumber == NULL);

    releaseNumbers(numbersBuffer, 1);
    freeNumber(sqrtB);
    return output;
}

__mpz_struct **getRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator) {

    RawRelation *relation = getRawRelation(instance, numbersBuffer, randomIntegerGenerator);
    __mpz_struct **output = allocateNumbersArray(instance->factorBase->length + 1, true);

    OrderedFactorListNode *currentRightSideRelationNodeList = relation->factorListOfRandomBSmoothNumber->head;

    int index = 0;

    if (instance->currentIndexCalculusAlgorithmStep == 2) {


        for (FactorBaseNode *currentFactorBaseNode = instance->factorBase->head;
             currentFactorBaseNode != NULL; currentFactorBaseNode = currentFactorBaseNode->next_node, index++) {

            if (currentRightSideRelationNodeList != NULL && mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                mpz_set(*(output + index), currentRightSideRelationNodeList->factor->exponent);
                currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

            } else
                mpz_set_ui(*(output + index), 0);
        }

        mpz_set(*(output + instance->factorBase->length), relation->randomExponentOfGenerator);
    }

    if (instance->currentIndexCalculusAlgorithmStep == 3) {

        for (FactorBaseNode *currentFactorBaseNode = instance->factorBase->head;
             currentFactorBaseNode != NULL; currentFactorBaseNode = currentFactorBaseNode->next_node, index++) {

            if (currentRightSideRelationNodeList != NULL && mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                mpz_set(*(output + index), currentRightSideRelationNodeList->factor->exponent);
                currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

            } else
                mpz_set_ui(*(output + index), 0);
        }

        mpz_set(*(output + instance->factorBase->length), relation->randomExponentOfGenerator);
        mpz_mul_si(*(output + instance->factorBase->length), *(output + instance->factorBase->length), -1);
    }

    freeRawRelation(relation);
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