#include "DLogProblemInstance.h"
#include "../Math/OrderedFactorList.h"
#include "../Math/Factorization.h"
#include "../Math/Number.h"
#include "../ThreadsPool/ThreadsPool.h"
#include <math.h>

typedef struct {

    OrderedFactorList *relationLeftSide;
    OrderedFactorList *relationRightSide;

} Relation;

Relation *allocateRelation() {

    Relation *output = malloc(sizeof(Relation));

    if (output == NULL) {
        exit(EXIT_FAILURE);
    } else {
        output->relationLeftSide = NULL;
        output->relationRightSide = NULL;
    }

    return output;
}

void deallocateRelation(Relation *input) {

    freeOrderedFactorList(input->relationLeftSide);
    freeOrderedFactorList(input->relationRightSide);
    free(input);
}

Relation *
getRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator,
            __mpz_struct *logarithmArgument) {

    Relation *output = allocateRelation();

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 1);

    __mpz_struct *power = buffer[0];

    __mpz_struct *sqrtB = allocateNumber();
    mpz_sqrt(sqrtB, instance->smoothnessBound);

    do {

        if (output->relationLeftSide != NULL)
            freeOrderedFactorList(output->relationLeftSide);


        output->relationLeftSide = allocateOrderedFactorList();


        __mpz_struct *currentPrimeNumberExponent = selectUniformlyDistributedRandomInteger(randomIntegerGenerator);

        appendFactor(output->relationLeftSide, allocateAndSetNumberFromNumber(instance->discreteLogarithm->base),
                     currentPrimeNumberExponent);

        mpz_powm(power, instance->discreteLogarithm->base, currentPrimeNumberExponent,
                 instance->discreteLogarithm->multiplicativeGroup);

        if (instance->currentIndexCalculusAlgorithmStep == 3) {
            mpz_mul(power, power, logarithmArgument);
        }
        mpz_mod(power, power, instance->discreteLogarithm->multiplicativeGroup);

        output->relationRightSide = factorizeCheckingBSmoothness(power, instance->smoothnessBound,
                                                                 mpz_get_ui(sqrtB), numbersBuffer);

    } while (output->relationRightSide == NULL);

    releaseNumbers(numbersBuffer, 1);

    //printOrderedFactorList(output->relationLeftSide);
    //printOrderedFactorList(output->relationRightSide);

    return output;
}

__mpz_struct **getLogarithmRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer,
                                    RandomIntegerGenerator *randomIntegerGenerator, __mpz_struct *logarithmArgument) {

    Relation *relation = getRelation(instance, numbersBuffer, randomIntegerGenerator, logarithmArgument);
    __mpz_struct **output = allocateNumbersArray(instance->factorBase->length + 1, true);

    OrderedFactorListNode *currentLeftSideRelationNodeList = relation->relationLeftSide->head;
    OrderedFactorListNode *currentRightSideRelationNodeList = relation->relationRightSide->head;

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

        mpz_set(*(output + instance->factorBase->length), currentLeftSideRelationNodeList->factor->exponent);
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

        mpz_set(*(output + instance->factorBase->length), currentLeftSideRelationNodeList->factor->exponent);
        mpz_mul_si(*(output + instance->factorBase->length), *(output + instance->factorBase->length), -1);



    }

    deallocateRelation(relation);
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

        __mpz_struct **relation = getLogarithmRelation(instance, numbersBuffer, randomIntegerGenerator, NULL);
        pushIntoCircularBuffer(circularBuffer, relation);
    }

    free(threadArgument);
    freeCircularBuffer(circularBuffer);
    freeNumbersBuffer(numbersBuffer);
    freeRandomIntegerGenerator(randomIntegerGenerator);

    return NULL;
}