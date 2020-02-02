//
// Created by andrea on 20/11/19.
//

#include <pthread.h>
#include "DLogProblemInstance.h"
#include "../Math/Number.h"
#include "../Math/OrderedFactorList.h"
#include "../Buffers/CircularBuffer.h"
#include "../ThreadsPool/ThreadsPool.h"
#include "RelationsRetrieval.h"


__mpz_struct *computeRequiredDiscreteLogarithmValue(__mpz_struct **relation, DLogProblemInstance* instance) {

    __mpz_struct *aux = retrieveNumberFromBuffer(instance->numbersBuffer);
    __mpz_struct *output = allocateAndSetNumberFromULL(0);

    for (unsigned long long index = 0; index < instance->factorBase->length; index++) {

        mpz_mul(aux, *(relation + index), *(instance->secondPhaseOutput->solution + index));
        mpz_add(output, output, aux);
    }

    mpz_mod(output, output, instance->moduloOfMultiplicativeGroupMinusOne);

    releaseNumber(instance->numbersBuffer);
    return output;
}


void startThirdPhase(DLogProblemInstance *instance) {

    instance->currentPhase = 3;
    instance->threadsPoolData->pauseCondition = false;

    pthread_cond_signal(&instance->threadsPoolData->pthreadCondition);

    __mpz_struct **relation = popFromCircularBuffer(instance->threadsPoolData->buffer);

    instance->discreteLogarithmToCompute->value = computeRequiredDiscreteLogarithmValue(relation, instance);

}