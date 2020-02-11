//
// Created by andrea on 25/11/19.
//

#include <pthread.h>
#include "DLogProblemInstance.h"
#include "../Math/OrderedFactorList.h"
#include "../Math/Factorization.h"
#include "../Math/Number.h"

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

    deallocateOrderedFactorList(input->relationLeftSide);
    deallocateOrderedFactorList(input->relationRightSide);
    free(input);
}

Relation *
getRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator,
            __mpz_struct *logarithmArgument) {

    Relation *output = allocateRelation();

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 2);

    __mpz_struct *randomNumber = buffer[0];
    __mpz_struct *power = buffer[1];

    do {

        if (output->relationLeftSide != NULL)
            deallocateOrderedFactorList(output->relationLeftSide);

        if (instance->currentIndexCalculusAlgorithmStep == 3) {
            mpz_set(randomNumber, logarithmArgument);
        } else {
            mpz_set_ui(randomNumber, 1);
        }

        output->relationLeftSide = allocateOrderedFactorList();

        for (FactorBaseNode *currentFactorBaseNode = instance->factorBase->head;
             currentFactorBaseNode != NULL; currentFactorBaseNode = currentFactorBaseNode->next_node) {

            __mpz_struct *currentPrimeNumber = allocateAndSetNumberFromNumber(currentFactorBaseNode->primeNumber);
            __mpz_struct *currentPrimeNumberExponent = selectUniformlyDistributedRandomInteger(randomIntegerGenerator);

            appendFactor(output->relationLeftSide, currentPrimeNumber, currentPrimeNumberExponent);

            mpz_pow_ui(power, currentPrimeNumber, mpz_get_ui(currentPrimeNumberExponent));
            mpz_mul(randomNumber, randomNumber, power);

            if (mpz_cmp(randomNumber, instance->discreteLogarithm->multiplicativeGroup) > 0)
                break;
        }

        mpz_mod(randomNumber, randomNumber, instance->discreteLogarithm->multiplicativeGroup);
        output->relationRightSide = factorizeCheckingBSmoothness(randomNumber, instance->smoothnessBound, numbersBuffer,
                                                                 randomIntegerGenerator);
        //output->relationRightSide = factorizeOptimizedCheckingBSmoothness(randomNumber, instance->discreteLogarithm->multiplicativeGroup, instance->smoothnessBound, numbersBuffer, randomIntegerGenerator);

    } while (output->relationRightSide == NULL);

    releaseNumbers(numbersBuffer, 2);

    return output;
}

__mpz_struct **getLogarithmRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer,
                                    RandomIntegerGenerator *randomIntegerGenerator, __mpz_struct *logarithmArgument) {

    Relation *relation = getRelation(instance, numbersBuffer, randomIntegerGenerator, logarithmArgument);
    __mpz_struct **output = allocateNumbersArray(instance->factorBase->length, true);

    __mpz_struct *zero = retrieveNumberFromBuffer(numbersBuffer);
    mpz_set_ui(zero, 0);

    __mpz_struct *currentLeftRelationPrime;
    __mpz_struct *currentLeftRelationPrimeExponent;

    __mpz_struct *currentRightRelationPrime;
    __mpz_struct *currentRightRelationPrimeExponent;

    OrderedFactorListNode *currentLeftSideRelationNodeList = relation->relationLeftSide->head;
    OrderedFactorListNode *currentRightSideRelationNodeList = relation->relationRightSide->head;

    int index = 0;

    for (FactorBaseNode *currentFactorBaseNode = instance->factorBase->head;
         currentFactorBaseNode != NULL; currentFactorBaseNode = currentFactorBaseNode->next_node) {

        if (currentLeftSideRelationNodeList != NULL) {
            currentLeftRelationPrime = currentLeftSideRelationNodeList->factor->base;
            currentLeftRelationPrimeExponent = currentLeftSideRelationNodeList->factor->exponent;
        } else
            currentLeftRelationPrime = zero;

        if (currentRightSideRelationNodeList != NULL) {
            currentRightRelationPrime = currentRightSideRelationNodeList->factor->base;
            currentRightRelationPrimeExponent = currentRightSideRelationNodeList->factor->exponent;
        } else
            currentRightRelationPrime = zero;

        if (instance->currentIndexCalculusAlgorithmStep == 2) {
            if (mpz_cmp(currentLeftRelationPrime, currentFactorBaseNode->primeNumber) != 0 &&
                mpz_cmp(currentRightRelationPrime, currentFactorBaseNode->primeNumber) != 0)
                mpz_set(*(output + index), zero);
            else if (mpz_cmp(currentLeftRelationPrime, currentRightRelationPrime) == 0) {

                mpz_sub(*(output + index), currentLeftRelationPrimeExponent, currentRightRelationPrimeExponent);

                currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
                currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

            } else if (mpz_cmp(currentLeftRelationPrime, currentFactorBaseNode->primeNumber) == 0) {

                mpz_set(*(output + index), currentLeftRelationPrimeExponent);
                currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;

            } else {

                mpz_mul_si(*(output + index), currentRightRelationPrimeExponent, -1);
                currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;
            }
        }

        if (instance->currentIndexCalculusAlgorithmStep == 3) {

            if (mpz_cmp(currentLeftRelationPrime, currentFactorBaseNode->primeNumber) != 0 &&
                mpz_cmp(currentRightRelationPrime, currentFactorBaseNode->primeNumber) != 0)
                mpz_set(*(output + index), zero);
            else if (mpz_cmp(currentLeftRelationPrime, currentRightRelationPrime) == 0) {

                mpz_sub(*(output + index), currentRightRelationPrimeExponent, currentLeftRelationPrimeExponent);

                currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
                currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

            } else if (mpz_cmp(currentRightRelationPrime, currentFactorBaseNode->primeNumber) == 0) {

                mpz_set(*(output + index), currentRightRelationPrimeExponent);
                currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

            } else {

                mpz_mul_si(*(output + index), currentLeftRelationPrimeExponent, -1);
                currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
            }
        }

        index++;
    }

    deallocateRelation(relation);
    releaseNumber(numbersBuffer);

    return output;
}


void *threadRoutineForRelationRetrieval(void *input) {

    ThreadsPoolData *threadsPoolData = (ThreadsPoolData *) input;
    DLogProblemInstance *instance = (DLogProblemInstance *) threadsPoolData->dLogProblemInstance;

    NumbersBuffer *numbersBuffer = allocateNumbersBuffer(instance->numbersBuffer->size);
    RandomIntegerGenerator *randomIntegerGenerator = allocateRandomIntegerGenerator(instance->maxRandomInteger);

    while (threadsPoolData->stoppingCondition != true) {

        __mpz_struct **relation = getLogarithmRelation(instance, numbersBuffer, randomIntegerGenerator, NULL);
        pushIntoCircularBuffer(threadsPoolData->sharedBuffer, relation);
    }

    freeNumbersBuffer(numbersBuffer);
    freeRandomIntegerGenerator(randomIntegerGenerator);

    return NULL;
}