//
// Created by andrea on 20/11/19.
//

#include "../DLogProblemInstance.h"
#include "../../Math/Number.h"
#include "../../Math/OrderedFactorList.h"

__mpz_struct *computeRequiredDiscreteLogarithmValue(__mpz_struct **relation, DLogProblemInstance *instance) {

    __mpz_struct *auxiliaryNumber = allocateNumber();
    __mpz_struct *output = allocateAndSetNumberFromULL(0);

    for (unsigned long long index = 0; index < instance->factorBase->length; index++) {

        mpz_mul(auxiliaryNumber, *(relation + index), *(instance->secondPhaseOutput->solution + index));
        mpz_add(output, output, auxiliaryNumber);
    }

    mpz_mod(output, output, instance->moduloOfMultiplicativeGroupMinusOne);

    deallocateNumber(auxiliaryNumber);
    return output;
}


void startThirdStep(DLogProblemInstance *instance) {

    sendSignalToThreadsPoolToExecuteSpecifiedAlgorithmStep(instance, 3);

    __mpz_struct **relation = popFromCircularBuffer(instance->threadsPoolData->sharedBuffer);

    instance->discreteLogarithm->value = computeRequiredDiscreteLogarithmValue(relation, instance);
}