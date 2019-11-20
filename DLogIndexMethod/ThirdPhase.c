//
// Created by andrea on 20/11/19.
//

#include "ThirdPhase.h"
#include "DLogProblemInstance.h"
#include "../Math/Number.h"
#include "../Math/OrderedFactorList.h"
#include "../Math/Factorization.h"

typedef struct {

    __mpz_struct **leftExponentValues;
    OrderedFactorList *rightFactors;

} Relation;

Relation *allocateRelation(unsigned long long leftSideRelationLength) {

    Relation* output = malloc(sizeof(Relation));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->leftExponentValues = allocateNumbersArray(leftSideRelationLength);
        output->rightFactors = NULL;

        return output;
    }
}

__mpz_struct* computeRequiredDiscreteLogarithmValue(DLogProblemInstance *instance, Relation *relation) {

    __mpz_struct *aux = retrieveAuxiliaryNumber(instance->applicationBuffer);
    __mpz_struct *output = allocateAndSetNumberFromULL(0);


    OrderedFactorListNode *currentFactorListNode = relation->rightFactors->head;

    for (unsigned long long index = 0; index < instance->factorBaseLength; index++) {

        __mpz_struct *leftBaseValue = *(instance->factorBase + index);
        __mpz_struct *leftExponentValue = *(relation->leftExponentValues + index);

        __mpz_struct *RightBaseValue = currentFactorListNode->factor->base;
        __mpz_struct *RightExponentValue = currentFactorListNode->factor->exponent;

        if (mpz_cmp(leftBaseValue, RightBaseValue) != 0) {
            RightExponentValue = allocateAndSetNumberFromULL(0);
        }

        mpz_sub(aux, RightExponentValue, leftExponentValue);
        mpz_mul(aux, aux, *(instance->secondPhaseOutput->solution + index));
        mpz_add(output, output, aux);
    }

    mpz_mod(output, output, instance->moduloOfMultiplicativeGroupMinusOne);

    releaseAuxiliaryNumber(instance->applicationBuffer, 1);
    return output;
}


Relation* computeRelation(DLogProblemInstance* instance) {

    Relation* output = allocateRelation(instance->factorBaseLength);

    __mpz_struct *randomProduct = retrieveAuxiliaryNumber(instance->applicationBuffer);
    __mpz_struct *powerModuloP = retrieveAuxiliaryNumber(instance->applicationBuffer);

    while (true) {

        mpz_set(randomProduct, instance->discreteLogarithmToCompute->argument);

        for (unsigned long long index = 0; index < instance->factorBaseLength; index++) {

            __mpz_struct *currentExponent = *(output->leftExponentValues + index);
            __mpz_struct *currentFactor = *(instance->factorBase + index);

            selectUniformlyDistributedRandomInteger(instance->randomIntegerGenerator, instance->maxRandomInteger,
                                                    currentExponent);
            mpz_powm(powerModuloP, currentFactor, currentExponent, instance->moduloOfMultiplicativeGroup);
            mpz_mul(randomProduct, randomProduct, powerModuloP);
            mpz_mod(randomProduct, randomProduct, instance->moduloOfMultiplicativeGroup);
        }

        output->rightFactors = factorizeCheckingBSmoothness(instance->applicationBuffer, randomProduct,
                                                                 instance->smoothnessBound);
        if (output->rightFactors != NULL)
            break;
    }

    releaseAuxiliaryNumber(instance->applicationBuffer, 2);
    return output;
}


void startThirdPhase(DLogProblemInstance* instance) {

    Relation *relation = computeRelation(instance);
    __mpz_struct* output = computeRequiredDiscreteLogarithmValue(instance, relation);

    gmp_printf("%Zd", output);
}