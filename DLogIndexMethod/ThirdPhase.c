//
// Created by andrea on 20/11/19.
//

#include "ThirdPhase.h"
#include "DLogProblemInstance.h"
#include "../Math/Number.h"
#include "../Math/OrderedFactorList.h"
#include "../Math/Factorization.h"
#include "../Buffers/CircularBuffer.h"
#include "../ThreadsPool/ThreadsPool.h"


__mpz_struct *computeRequiredDiscreteLogarithmValue(__mpz_struct **relation, DLogProblemInstance* instance) {

    __mpz_struct *aux = retrieveAuxiliaryNumber(instance->applicationBuffer);
    __mpz_struct *output = allocateAndSetNumberFromULL(0);

    for (unsigned long long index = 0; index < instance->factorBase->length; index++) {

        mpz_mul(aux, *(relation + index), *(instance->secondPhaseOutput->solution + index));
        mpz_add(output, output, aux);
    }

    mpz_mod(output, output, instance->moduloOfMultiplicativeGroupMinusOne);

    releaseAuxiliaryNumber(instance->applicationBuffer, 1);
    return output;
}

__mpz_struct **getNewRelation(ApplicationBuffer* threadAppBuffer, RandomIntegerGenerator* threadRandIntGen, DLogProblemInstance *instance) {

    __mpz_struct **outputRelation = allocateNumbersArray(instance->factorBase->length, true);

    __mpz_struct *randomProduct = retrieveAuxiliaryNumber(threadAppBuffer);
    __mpz_struct *powerModuloP = retrieveAuxiliaryNumber(threadAppBuffer);

    OrderedFactorList *factorListOfBSmoothNumber = NULL;

    while (factorListOfBSmoothNumber == NULL) {
        __mpz_struct *currentExponent;
        __mpz_struct *currentFactor;

        mpz_set(randomProduct, instance->discreteLogarithmToCompute->argument);

        FactorBaseNode *currentFactorBaseNode = instance->factorBase->head;

        for (unsigned long long index = 0; index < instance->factorBase->length; index++) {

            currentExponent = *(outputRelation + index);
            currentFactor = currentFactorBaseNode->primeNumber;

            selectUniformlyDistributedRandomInteger(threadRandIntGen, currentExponent);
            mpz_powm(powerModuloP, currentFactor, currentExponent, instance->moduloOfMultiplicativeGroup);
            mpz_mul(randomProduct, randomProduct, powerModuloP);
            mpz_mod(randomProduct, randomProduct, instance->moduloOfMultiplicativeGroup);

            currentFactorBaseNode = currentFactorBaseNode->next_node;
        }

        factorListOfBSmoothNumber = factorizeCheckingBSmoothness(threadAppBuffer, randomProduct, instance->smoothnessBound);
    }

    releaseAuxiliaryNumber(threadAppBuffer, 2);

    // ============================ //

    __mpz_struct *leftBaseValue;
    __mpz_struct *leftExponentValue;
    __mpz_struct *RightBaseValue;
    __mpz_struct *RightExponentValue;

    OrderedFactorListNode *currentFactorNodeRight = factorListOfBSmoothNumber->head;
    FactorBaseNode *currentFactorNodeLeft = instance->factorBase->head;

    for (unsigned long long index = 0; index < instance->factorBase->length; index++) {

        leftBaseValue = currentFactorNodeLeft->primeNumber;
        leftExponentValue = *(outputRelation + index);

        RightBaseValue = currentFactorNodeRight->factor->base;
        RightExponentValue = currentFactorNodeRight->factor->exponent;

        if (mpz_cmp(leftBaseValue, RightBaseValue) != 0) {
            RightExponentValue = allocateAndSetNumberFromULL(0);
        } else {
            if (currentFactorNodeRight->next_node != NULL)
                currentFactorNodeRight = currentFactorNodeRight->next_node;
        }

        mpz_sub(*(outputRelation + index), RightExponentValue, leftExponentValue);

        currentFactorNodeLeft = currentFactorNodeLeft->next_node;
    }

    return outputRelation;
}



void *produceRelation(void *input) {

    ThreadsPoolData* threadsPoolData = (ThreadsPoolData *) input;
    DLogProblemInstance *instance = threadsPoolData->threadData;

    ApplicationBuffer* threadAppBuffer = allocateApplicationBuffer();
    RandomIntegerGenerator* threadRandIntGen = allocateRandomIntegerGenerator(instance->maxRandomInteger);

    while (threadsPoolData->stoppingCondition != true) {

        __mpz_struct **relation = getNewRelation(threadAppBuffer, threadRandIntGen, instance);

        pushCircularBuffer(threadsPoolData->buffer, relation);
    }

    deallocateApplicationBuffer(threadAppBuffer);
    deallocateRandomIntegerGenerator(threadRandIntGen);

    return NULL;
}

void startThirdPhase(DLogProblemInstance *instance) {

    ThreadsPoolData *threadsPoolData = allocateThreadsPoolData(instance);

    startThreadsPool(1, &produceRelation, (void *) threadsPoolData);

    __mpz_struct **relation = popCircularBuffer(threadsPoolData->buffer);
    __mpz_struct *output = computeRequiredDiscreteLogarithmValue22222(relation, instance);

            /*
    Relation *relation = computeRelation(instance);
    __mpz_struct *output = computeRequiredDiscreteLogarithmValue(instance, relation);
*/
    gmp_printf("%Zd", output);
}