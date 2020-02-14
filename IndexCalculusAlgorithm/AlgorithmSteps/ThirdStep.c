#include "../DLogProblemInstance.h"
#include "../../Math/Number.h"
#include "../../Math/OrderedFactorList.h"
#include "../RelationsRetrieval.h"

__mpz_struct *computeRequiredDiscreteLogarithmValue(__mpz_struct **relation, DLogProblemInstance *instance) {

    __mpz_struct *auxiliaryNumber = retrieveNumberFromBuffer(instance->numbersBuffer);
    __mpz_struct *output = allocateAndSetNumberFromULL(0);

    for (unsigned long long index = 0; index < instance->factorBase->length; index++) {

        __mpz_struct *exp = *(relation + index);
        __mpz_struct *logValue = *(instance->solutionOfSecondPhase + index);
        mpz_mul(auxiliaryNumber, exp, logValue);
        mpz_add(output, output, auxiliaryNumber);
    }

    mpz_add(output, output, *(relation + instance->factorBase->length));
    mpz_mod(output, output, instance->discreteLogarithm->multiplicativeGroupMinusOne);

    releaseNumber(instance->numbersBuffer);
    return output;
}

void startThirdStep(DLogProblemInstance *instance) {

    instance->currentIndexCalculusAlgorithmStep = 3;

    __mpz_struct** relation = getRelation(instance, instance->numbersBuffer, instance->randomIntegerGenerator);

    instance->discreteLogarithm->value = computeRequiredDiscreteLogarithmValue(relation, instance);

    freeNumbersArray(relation, instance->factorBase->length);
}