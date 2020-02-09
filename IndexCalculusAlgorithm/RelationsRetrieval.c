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

Relation *getRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator) {

    Relation *output = allocateRelation();

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 2);

    __mpz_struct *randomNumber = buffer[0];
    __mpz_struct *power = buffer[1];

    do {

        if (output->relationLeftSide != NULL)
            deallocateOrderedFactorList(output->relationLeftSide);

        if (instance->currentIndexCalculusAlgorithmStep == 3) {
            mpz_set(randomNumber, instance->discreteLogarithm->argument);
        } else {
            mpz_set_ui(randomNumber, 1);
        }

        output->relationLeftSide = allocateOrderedFactorList();

        for (FactorBaseNode *currentFactorBaseNode = instance->factorBase->head; currentFactorBaseNode != NULL; currentFactorBaseNode = currentFactorBaseNode->next_node) {

            __mpz_struct *currentPrimeNumber = allocateAndSetNumberFromNumber(currentFactorBaseNode->primeNumber);
            __mpz_struct *currentPrimeNumberExponent = selectUniformlyDistributedRandomInteger(randomIntegerGenerator);

            appendFactor(output->relationLeftSide, currentPrimeNumber, currentPrimeNumberExponent);

            mpz_pow_ui(power, currentPrimeNumber, mpz_get_ui(currentPrimeNumberExponent));
            mpz_mul(randomNumber, randomNumber, power);

            if (mpz_cmp(randomNumber, instance->discreteLogarithm->multiplicativeGroup) > 0)
                break;
        }

        mpz_mod(randomNumber, randomNumber, instance->discreteLogarithm->multiplicativeGroup);
        output->relationRightSide = factorizeCheckingBSmoothness(randomNumber, instance->smoothnessBound, numbersBuffer, randomIntegerGenerator);
    } while (output->relationRightSide == NULL);

    releaseNumbers(numbersBuffer, 2);
    return output;
}

__mpz_struct **getLogarithmRelation(Relation *relation, DLogProblemInstance *instance, NumbersBuffer *numbersBuffer) {

    __mpz_struct **output = allocateNumbersArray(instance->factorBase->length + 1, true);

    __mpz_struct *zero = retrieveNumberFromBuffer(numbersBuffer);
    mpz_set_ui(zero, 0);

    __mpz_struct *currentLeftRelationPrime;
    __mpz_struct *currentLeftRelationPrimeExponent;

    __mpz_struct *currentRightRelationPrime;
    __mpz_struct *currentRightRelationPrimeExponent;

    OrderedFactorListNode *currentLeftSideRelationNodeList = relation->relationLeftSide->head;
    OrderedFactorListNode *currentRightSideRelationNodeList = relation->relationRightSide->head;

    int index = 0;

    for (FactorBaseNode *currentFactorBaseNode = instance->factorBase->head; currentFactorBaseNode != NULL; currentFactorBaseNode = currentFactorBaseNode->next_node) {

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
            if (mpz_cmp(currentLeftRelationPrime, currentFactorBaseNode->primeNumber) != 0 && mpz_cmp(currentRightRelationPrime, currentFactorBaseNode->primeNumber) != 0)
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

            if (mpz_cmp(currentLeftRelationPrime, currentFactorBaseNode->primeNumber) != 0 && mpz_cmp(currentRightRelationPrime, currentFactorBaseNode->primeNumber) != 0)
                mpz_set(*(output + index), zero);
            else if (mpz_cmp(currentLeftRelationPrime, currentRightRelationPrime) == 0) {

                mpz_sub(*(output + index), currentRightRelationPrimeExponent, currentLeftRelationPrimeExponent);

                currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
                currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

            } else if (mpz_cmp(currentRightRelationPrime, currentFactorBaseNode->primeNumber) == 0) {

                mpz_set(*(output + index), currentRightRelationPrimeExponent);
                currentLeftSideRelationNodeList = currentRightSideRelationNodeList->next_node;

            } else {

                mpz_mul_si(*(output + index), currentLeftRelationPrimeExponent, -1);
                currentRightSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
            }
        }

        index++;
    }

    deallocateRelation(relation);
    releaseNumber(numbersBuffer);

    return output;
}

void printRelation(__mpz_struct **relation, DLogProblemInstance *instance) {

    for (int index = 0; index < instance->factorBase->length; index++)
        gmp_fprintf(stderr, " %Zd + ", *(relation + index));
    fprintf(stderr, "\n");
}

void *threadRoutineForRelationRetrieval(void *input) {

    ThreadsPoolData *threadsPoolData = (ThreadsPoolData *) input;
    DLogProblemInstance *instance = (DLogProblemInstance *) threadsPoolData->dLogProblemInstance;

    NumbersBuffer *numbersBuffer = allocateNumbersBuffer(instance->numbersBuffer->size);
    RandomIntegerGenerator *randomIntegerGenerator = allocateRandomIntegerGenerator(instance->maxRandomInteger);

    while (threadsPoolData->stoppingCondition != true) {

        while (threadsPoolData->pauseCondition != true) {

            Relation *rawRelation = getRelation(instance, numbersBuffer, randomIntegerGenerator);

            /*
            fprintf(stderr, "From relation:\n");
            printOrderedFactorList(rawRelation->relationLeftSide);
            fprintf(stderr, " = ");
            printOrderedFactorList(rawRelation->relationRightSide);
            fprintf(stderr, "\n");
*/
            __mpz_struct **relation = getLogarithmRelation(rawRelation, instance, numbersBuffer);

            //printRelation(relation, instance);

            pushIntoCircularBuffer(threadsPoolData->sharedBuffer, relation);
        }

        pthread_cond_wait(&threadsPoolData->pthreadCondition, &threadsPoolData->pthreadMutex);
    }

    freeNumbersBuffer(numbersBuffer);
    deallocateRandomIntegerGenerator(randomIntegerGenerator);

    return NULL;
}