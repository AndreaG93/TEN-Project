#include <stdlib.h>
#include "SecondStep.h"
#include "../../Math/Number.h"
#include "../../Math/Matrix.h"
#include "../RelationsRetrieval.h"
#include "../../ThreadsPool/ThreadsPool.h"

__mpz_struct **populateSecondPhaseOutput(DLogProblemInstance *instance, Matrix *resolvedEquationSystem) {

    __mpz_struct **output = allocateNumbersArray(instance->factorBase->length, true);

    __mpz_struct *auxiliary = retrieveNumberFromBuffer(instance->numbersBuffer);

    mpz_div_ui(auxiliary, instance->discreteLogarithm->multiplicativeGroupMinusOne, 2);
    mpz_set(*(output), auxiliary);

    unsigned long long columnIndex = resolvedEquationSystem->columnLength - 1;
    unsigned long long outputIndex = 1;

    for (unsigned long long rowIndex = 0; rowIndex < instance->factorBase->length - 1; rowIndex++, outputIndex++) {
        __mpz_struct *number = getNumberMatrixCell(resolvedEquationSystem, rowIndex, columnIndex);
        mpz_set(*(output + outputIndex), number);
    }

    releaseNumber(instance->numbersBuffer);
    return output;
}



void startSecondStep(DLogProblemInstance *instance) {

    unsigned long totalRow = instance->factorBase->length * 1.25;

    Matrix *equationSystem = allocateMatrix(totalRow, instance->factorBase->length + 1);

    pthread_t *pthreads = startThreadPool(instance->threadsPoolSize, &threadRoutineForRelationRetrieval, (void *) instance->threadsPoolData);

    fprintf(stderr, "--> Producing relations...\n");

    for (unsigned long long currentRow = 0; currentRow != totalRow; currentRow++) {

        __mpz_struct **relation;

        do {
            relation = dequeue(instance->threadsPoolData->semiLockFreeQueue);
        } while (relation == NULL);

        for (unsigned long long currentColumn = 0; currentColumn < instance->factorBase->length + 1; currentColumn++) {

            __mpz_struct *currentNumber = *(relation + currentColumn);
            setNumberMatrixCell(equationSystem, currentRow, currentColumn, currentNumber);
        }
        free(relation);
    }

    stopThreadsPool(instance);
    joinAndFreeThreadsPool(pthreads, instance->threadsPoolSize);

    fprintf(stderr, "--> Resolving equation system...\n");
    performGaussianElimination(equationSystem, instance->numbersBuffer, instance->discreteLogarithm->multiplicativeGroupMinusOne, instance->threadsPoolSize);

#ifdef DEBUG
    printMatrix(equationSystem);
#endif

    instance->solutionOfSecondPhase = populateSecondPhaseOutput(instance, equationSystem);

    freeMatrix(equationSystem);
}