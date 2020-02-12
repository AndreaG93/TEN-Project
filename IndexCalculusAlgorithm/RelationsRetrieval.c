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

Relation *getRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator, __mpz_struct *logarithmArgument) {

    Relation *output = allocateRelation();

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 2);

    __mpz_struct *randomNumber = buffer[0];
    __mpz_struct *power = buffer[1];

    do {

        if (output->relationLeftSide != NULL)
            freeOrderedFactorList(output->relationLeftSide);

        if (instance->currentIndexCalculusAlgorithmStep == 3) {
            mpz_set(randomNumber, logarithmArgument);
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

        output->relationRightSide = factorizeOptimizedCheckingBSmoothness(randomNumber, instance->discreteLogarithm->multiplicativeGroup, instance->smoothnessBound, numbersBuffer, randomIntegerGenerator);

    } while (output->relationRightSide == NULL);

    releaseNumbers(numbersBuffer, 2);

    return output;
}

__mpz_struct **getLogarithmRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator, __mpz_struct *logarithmArgument) {

    Relation *relation = getRelation(instance, numbersBuffer, randomIntegerGenerator, logarithmArgument);
    __mpz_struct **output = allocateNumbersArray(instance->factorBase->length, true);

    OrderedFactorListNode *currentLeftSideRelationNodeList = relation->relationLeftSide->head;
    OrderedFactorListNode *currentRightSideRelationNodeList = relation->relationRightSide->head;

    int index = 0;

    for (FactorBaseNode *currentFactorBaseNode = instance->factorBase->head; currentFactorBaseNode != NULL; currentFactorBaseNode = currentFactorBaseNode->next_node, index++) {

        if (instance->currentIndexCalculusAlgorithmStep == 2) {

            if (currentLeftSideRelationNodeList != NULL && currentRightSideRelationNodeList != NULL) {

                if (mpz_cmp(currentLeftSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0 && mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_sub(*(output + index), currentLeftSideRelationNodeList->factor->exponent, currentRightSideRelationNodeList->factor->exponent);

                    currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
                    currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

                } else if (mpz_cmp(currentLeftSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_set(*(output + index), currentLeftSideRelationNodeList->factor->exponent);
                    currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;

                } else if (mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_mul_si(*(output + index), currentRightSideRelationNodeList->factor->exponent, -1);
                    currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

                } else
                    mpz_set_ui(*(output + index), 0);

            } else if (currentLeftSideRelationNodeList != NULL) {

                if (mpz_cmp(currentLeftSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_set(*(output + index), currentLeftSideRelationNodeList->factor->exponent);
                    currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
                } else
                    mpz_set_ui(*(output + index), 0);

            } else if (currentRightSideRelationNodeList != NULL) {

                if (mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_mul_si(*(output + index), currentRightSideRelationNodeList->factor->exponent, -1);
                    currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;
                } else
                    mpz_set_ui(*(output + index), 0);

            } else
                mpz_set_ui(*(output + index), 0);
        }

        if (instance->currentIndexCalculusAlgorithmStep == 3) {

            if (currentLeftSideRelationNodeList != NULL && currentRightSideRelationNodeList != NULL) {

                if (mpz_cmp(currentLeftSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0 && mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_sub(*(output + index), currentRightSideRelationNodeList->factor->exponent, currentLeftSideRelationNodeList->factor->exponent);

                    currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
                    currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

                } else if (mpz_cmp(currentLeftSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_mul_si(*(output + index), currentLeftSideRelationNodeList->factor->exponent, -1);
                    currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;

                } else if (mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_set(*(output + index), currentRightSideRelationNodeList->factor->exponent);
                    currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;

                } else
                    mpz_set_ui(*(output + index), 0);

            } else if (currentLeftSideRelationNodeList != NULL) {

                if (mpz_cmp(currentLeftSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_mul_si(*(output + index), currentLeftSideRelationNodeList->factor->exponent, -1);
                    currentLeftSideRelationNodeList = currentLeftSideRelationNodeList->next_node;
                } else
                    mpz_set_ui(*(output + index), 0);

            } else if (currentRightSideRelationNodeList != NULL) {

                if (mpz_cmp(currentRightSideRelationNodeList->factor->base, currentFactorBaseNode->primeNumber) == 0) {

                    mpz_set(*(output + index), currentRightSideRelationNodeList->factor->exponent);
                    currentRightSideRelationNodeList = currentRightSideRelationNodeList->next_node;
                } else
                    mpz_set_ui(*(output + index), 0);

            } else
                mpz_set_ui(*(output + index), 0);
        }
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
    RandomIntegerGenerator *randomIntegerGenerator = allocateRandomIntegerGenerator(instance->maxRandomInteger);
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