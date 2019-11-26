//
// Created by andrea on 25/11/19.
//

#include <pthread.h>
#include "RelationsRetrieval.h"
#include "../ThreadsPool/ThreadsPool.h"
#include "DLogProblemInstance.h"
#include "../Math/OrderedFactorList.h"
#include "../Math/Factorization.h"
#include "../Math/Number.h"


OrderedFactorList* getFactorListOfRandomBSmoothNumber(__mpz_struct **ancestorRelation, ApplicationBuffer* appBuffer, RandomIntegerGenerator* randIntGen, DLogProblemInstance *instance) {

    OrderedFactorList *output = NULL;

    __mpz_struct *randomProduct = retrieveAuxiliaryNumber(appBuffer);
    __mpz_struct *powerModuloP = retrieveAuxiliaryNumber(appBuffer);

    while (output == NULL) {
        __mpz_struct *currentExponent;
        __mpz_struct *currentFactor;

        if (instance->currentPhase == 3) {
            mpz_set(randomProduct, instance->discreteLogarithmToCompute->argument);
        } else {
            mpz_set_ui(randomProduct, 1);
        }

        FactorBaseNode *currentFactorBaseNode = instance->factorBase->head;

        for (unsigned long long index = 0; index < instance->factorBase->length; index++) {

            currentExponent = *(ancestorRelation + index);
            currentFactor = currentFactorBaseNode->primeNumber;

            selectUniformlyDistributedRandomInteger(randIntGen, currentExponent);
            mpz_powm(powerModuloP, currentFactor, currentExponent, instance->moduloOfMultiplicativeGroup);
            mpz_mul(randomProduct, randomProduct, powerModuloP);
            mpz_mod(randomProduct, randomProduct, instance->moduloOfMultiplicativeGroup);

            currentFactorBaseNode = currentFactorBaseNode->next_node;
        }

        output = factorizeCheckingBSmoothness(appBuffer, randomProduct, instance->smoothnessBound);
    }

    releaseAuxiliaryNumber(appBuffer, 2);

    return output;
}


void populateRelation(__mpz_struct **ancestorRelation, OrderedFactorList* factorListOfBSmoothNumber, DLogProblemInstance *instance) {

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

        if (instance->currentPhase == 3) {
            mpz_sub(*(ancestorRelation + index), RightExponentValue, leftExponentValue);
        } else {
            mpz_sub(*(ancestorRelation + index), leftExponentValue, RightExponentValue);
        }

        currentFactorNodeLeft = currentFactorNodeLeft->next_node;
    }
}

__mpz_struct **getNewRelation(ApplicationBuffer* threadAppBuffer, RandomIntegerGenerator* threadRandIntGen, DLogProblemInstance *instance) {

    __mpz_struct **outputRelation = allocateNumbersArray(instance->factorBase->length, true);

    OrderedFactorList *factorListOfBSmoothNumber = getFactorListOfRandomBSmoothNumber(outputRelation, threadAppBuffer, threadRandIntGen, instance);
    populateRelation(outputRelation, factorListOfBSmoothNumber, instance);

    return outputRelation;
}


void *threadRoutineForRelationRetrieval(void *input) {

    ThreadsPoolData* threadsPoolData = (ThreadsPoolData *) input;
    DLogProblemInstance *instance = threadsPoolData->threadData;

    ApplicationBuffer* threadAppBuffer = allocateApplicationBuffer();
    RandomIntegerGenerator* threadRandIntGen = allocateRandomIntegerGenerator(instance->maxRandomInteger);

    while (threadsPoolData->stoppingCondition != true) {

        while (threadsPoolData->pauseCondition != true) {
            __mpz_struct **relation = getNewRelation(threadAppBuffer, threadRandIntGen, instance);

            pushIntoCircularBuffer(threadsPoolData->buffer, relation);
        }

        pthread_cond_wait(&threadsPoolData->pthreadCondition, &threadsPoolData->pthreadMutex);
    }

    deallocateApplicationBuffer(threadAppBuffer);
    deallocateRandomIntegerGenerator(threadRandIntGen);

    return NULL;
}