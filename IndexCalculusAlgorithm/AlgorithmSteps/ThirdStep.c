//
// Created by andrea on 20/11/19.
//

#include "../DLogProblemInstance.h"
#include "../../Math/Number.h"
#include "../../Math/OrderedFactorList.h"
#include "../RelationsRetrieval.h"

__mpz_struct *computeRequiredDiscreteLogarithmValue(__mpz_struct **relation, DLogProblemInstance *instance) {

    __mpz_struct *auxiliaryNumber = retrieveNumberFromBuffer(instance->numbersBuffer);
    __mpz_struct *output = allocateAndSetNumberFromULL(0);

    for (unsigned long long index = 0; index < instance->factorBase->length; index++) {

        mpz_mul(auxiliaryNumber, *(relation + index), *(instance->secondPhaseOutput->solution + index));
        mpz_add(output, output, auxiliaryNumber);
    }

    mpz_mod(output, output, instance->discreteLogarithm->multiplicativeGroupMinusOne);

    releaseNumber(instance->numbersBuffer);
    return output;
}


void startThirdStep(DLogProblemInstance *instance) {

    instance->currentIndexCalculusAlgorithmStep = 3;
    __mpz_struct **relation;
    __mpz_struct *finalOutput;

    relation = getLogarithmRelation(instance, instance->numbersBuffer, instance->randomIntegerGenerator, instance->discreteLogarithm->argument);
    finalOutput = computeRequiredDiscreteLogarithmValue(relation, instance);

    if (mpz_cmp(instance->secondPhaseOutput->base, instance->discreteLogarithm->base) == 0) {
        instance->discreteLogarithm->value = finalOutput;
    } else {

        freeNumbersArray(relation, instance->factorBase->length);

        __mpz_struct *numerator = finalOutput;
        __mpz_struct *denominator;


        relation = getLogarithmRelation(instance, instance->numbersBuffer, instance->randomIntegerGenerator, instance->discreteLogarithm->base);
        denominator = computeRequiredDiscreteLogarithmValue(relation, instance);

        mpz_invert(denominator, denominator, instance->discreteLogarithm->multiplicativeGroupMinusOne);
        mpz_mul(instance->discreteLogarithm->value, numerator, denominator);
        mpz_mod(instance->discreteLogarithm->value, instance->discreteLogarithm->value, instance->discreteLogarithm->multiplicativeGroupMinusOne);
    }
}