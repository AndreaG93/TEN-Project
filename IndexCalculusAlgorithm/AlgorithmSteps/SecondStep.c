#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "SecondStep.h"
#include "../../Math/Number.h"
#include "../../Math/Matrix.h"
#include "../../Math/Common.h"
#include "../RelationsRetrieval.h"
#include "../../ThreadsPool/ThreadsPool.h"

SecondPhaseOutput *allocateSecondPhaseOutput(unsigned long long size) {

    SecondPhaseOutput *output = malloc(sizeof(SecondPhaseOutput));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        output->solution = allocateNumbersArray(size, true);


    return output;
}

SecondPhaseOutput *populateSecondPhaseOutput(DLogProblemInstance *instance, Matrix *resolvedEquationSystem) {

    __mpz_struct* aux = retrieveNumberFromBuffer(instance->numbersBuffer);

    SecondPhaseOutput* output = allocateSecondPhaseOutput(instance->factorBase->length);

    mpz_div_ui(aux, instance->discreteLogarithm->multiplicativeGroupMinusOne, 2);
    mpz_set(*(output->solution), aux);


    unsigned long long columnIndex = resolvedEquationSystem->columnLength - 1;
    unsigned long long outputIndex = 1;

    for (unsigned long long rowIndex = 0; rowIndex < instance->factorBase->length - 1; rowIndex++, outputIndex++) {
        __mpz_struct *number = getNumberMatrixCell(resolvedEquationSystem, rowIndex, columnIndex);
        mpz_set(*(output->solution + outputIndex), number);
    }

    releaseNumber(instance->numbersBuffer);
    return output;
}

void startSecondStep(DLogProblemInstance *instance) {

    unsigned long totalRow = instance->factorBase->length*2;

    Matrix *equationSystem = allocateMatrix(totalRow, instance->factorBase->length + 1);

    pthread_t *pthreads = startThreadPool(instance->threadsPoolSize, &threadRoutineForRelationRetrieval, (void *) instance->threadsPoolData);

    fprintf(stderr, "--> Producing relations...\n");

    for (unsigned long long currentRow = 0; currentRow != totalRow; currentRow++) {

        __mpz_struct **relation = popFromArrayOfCircularBufferRoundRobinManner(instance);

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
    printMatrix(equationSystem);

    instance->secondPhaseOutput = populateSecondPhaseOutput(instance, equationSystem);

    freeMatrix(equationSystem);
}