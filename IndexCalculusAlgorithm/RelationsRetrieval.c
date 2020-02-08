//
// Created by andrea on 25/11/19.
//

#include <pthread.h>
#include "DLogProblemInstance.h"
#include "../Math/OrderedFactorList.h"
#include "../Math/Factorization.h"
#include "../Math/Number.h"

OrderedFactorList *getFactorListOfRandomBSmoothNumber(__mpz_struct **ancestorRelation, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randIntGen, DLogProblemInstance *instance) {

    OrderedFactorList *output = NULL;

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 2);
    __mpz_struct *randomProduct = buffer[0];
    __mpz_struct *powerModuloP = buffer[1];

    while (output == NULL) {
        __mpz_struct *currentExponent;
        __mpz_struct *currentFactor;

        if (instance->currentIndexCalculusAlgorithmStep == 3) {
            mpz_set(randomProduct, instance->discreteLogarithm->argument);
        } else {
            mpz_set_ui(randomProduct, 1);
        }

        FactorBaseNode *currentFactorBaseNode = instance->factorBase->head;

        for (unsigned long long index = 0; index < instance->factorBase->length; index++) {

            currentExponent = *(ancestorRelation + index);
            currentFactor = currentFactorBaseNode->primeNumber;

            selectUniformlyDistributedRandomInteger(randIntGen, currentExponent);
            mpz_powm(powerModuloP, currentFactor, currentExponent, instance->discreteLogarithm->multiplicativeGroup);
            mpz_mul(randomProduct, randomProduct, powerModuloP);
            mpz_mod(randomProduct, randomProduct, instance->discreteLogarithm->multiplicativeGroup);

            currentFactorBaseNode = currentFactorBaseNode->next_node;
        }

        output = factorize(randomProduct, numbersBuffer, randIntGen);
    }

    releaseNumbers(numbersBuffer, 2);

    return output;
}


void populateRelation(__mpz_struct **ancestorRelation, OrderedFactorList *factorListOfBSmoothNumber, DLogProblemInstance *instance) {

    __mpz_struct *leftBaseValue;
    __mpz_struct *leftExponentValue;
    __mpz_struct *RightBaseValue;
    __mpz_struct *RightExponentValue;

    OrderedFactorListNode *currentFactorNodeRight = factorListOfBSmoothNumber->head;
    FactorBaseNode *currentFactorNodeLeft = instance->factorBase->head;

    for (unsigned long long index = 0; index < instance->factorBase->length; index++) {

        leftBaseValue = currentFactorNodeLeft->primeNumber;
        leftExponentValue = *(ancestorRelation + index);

        RightBaseValue = currentFactorNodeRight->factor->base;
        RightExponentValue = currentFactorNodeRight->factor->exponent;

        if (mpz_cmp(leftBaseValue, RightBaseValue) != 0) {
            RightExponentValue = allocateAndSetNumberFromULL(0);
        } else {
            if (currentFactorNodeRight->next_node != NULL)
                currentFactorNodeRight = currentFactorNodeRight->next_node;
        }

        if (instance->currentIndexCalculusAlgorithmStep == 3) {
            mpz_sub(*(ancestorRelation + index), RightExponentValue, leftExponentValue);
            mpz_mod(*(ancestorRelation + index), *(ancestorRelation + index), instance->discreteLogarithm->multiplicativeGroupMinusOne);
        } else {
            mpz_sub(*(ancestorRelation + index), leftExponentValue, RightExponentValue);
            mpz_mod(*(ancestorRelation + index), *(ancestorRelation + index), instance->discreteLogarithm->multiplicativeGroupMinusOne);
        }

        currentFactorNodeLeft = currentFactorNodeLeft->next_node;
    }
}

__mpz_struct **getRelation(DLogProblemInstance *instance, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator) {

    __mpz_struct **outputRelation = allocateNumbersArray(instance->factorBase->length, true);

    OrderedFactorList *factorListOfBSmoothNumber = getFactorListOfRandomBSmoothNumber(outputRelation, numbersBuffer, randomIntegerGenerator, instance);
    populateRelation(outputRelation, factorListOfBSmoothNumber, instance);

    return outputRelation;
}


void *threadRoutineForRelationRetrieval(void *input) {

    ThreadsPoolData *threadsPoolData = (ThreadsPoolData *) input;
    DLogProblemInstance *instance = (DLogProblemInstance *) threadsPoolData->dLogProblemInstance;

    NumbersBuffer *numbersBuffer = allocateNumbersBuffer(instance->numbersBuffer->size);
    RandomIntegerGenerator *randomIntegerGenerator = allocateRandomIntegerGenerator(instance->maxRandomInteger);

    while (threadsPoolData->stoppingCondition != true) {

        while (threadsPoolData->pauseCondition != true) {

            __mpz_struct **relation = getRelation(instance, numbersBuffer, randomIntegerGenerator);
            pushIntoCircularBuffer(threadsPoolData->sharedBuffer, relation);
        }

        pthread_cond_wait(&threadsPoolData->pthreadCondition, &threadsPoolData->pthreadMutex);
    }

    freeNumbersBuffer(numbersBuffer);
    deallocateRandomIntegerGenerator(randomIntegerGenerator);

    return NULL;
}