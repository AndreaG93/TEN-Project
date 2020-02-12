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
    else {
        output->solution = allocateNumbersArray(size, true);
        output->base = allocateNumber();
        output->indexOfBaseRespectToFactorBase = 0;
    }

    return output;
}

void populateSecondPhaseOutput(Matrix *resolvedEquationSystem, SecondPhaseOutput *allocatedSecondPhaseOutput) {


    bool found = true;
    unsigned long long columnIndex = resolvedEquationSystem->columnLength - 1;
    unsigned long long outputIndex = 0;
    for (unsigned long long rowIndex = 0; rowIndex < resolvedEquationSystem->rowLength; rowIndex++) {

        __mpz_struct *number;

        if (allocatedSecondPhaseOutput->indexOfBaseRespectToFactorBase == rowIndex && found) {
            rowIndex--;
            found = false;
        } else {
            number = getNumberMatrixCell(resolvedEquationSystem, rowIndex, columnIndex);
            mpz_set(*(allocatedSecondPhaseOutput->solution + outputIndex), number);
        }

        outputIndex++;
        if (outputIndex == columnIndex)
            break;
    }
}

SecondPhaseOutput *getBaseToComputeKnownLogarithm(DLogProblemInstance *instance) {

    SecondPhaseOutput *output = allocateSecondPhaseOutput(instance->factorBase->length);
    FactorBaseNode *currentNode;

    unsigned long index = 0;

    if (mpz_cmp(instance->discreteLogarithm->base, instance->factorBase->tail->primeNumber) <= 0 && isPrime(instance->discreteLogarithm->base)) {

        for (currentNode = instance->factorBase->head; currentNode != NULL; currentNode = currentNode->next_node, index++)
            if (mpz_cmp(instance->discreteLogarithm->base, currentNode->primeNumber) == 0)
                break;

    } else
        for (currentNode = instance->factorBase->head; currentNode != NULL; currentNode = currentNode->next_node, index++)
            if (isGroupGenerator(currentNode->primeNumber, instance->discreteLogarithm->multiplicativeGroup, instance->numbersBuffer, instance->randomIntegerGenerator, true))
                break;

    if (currentNode == NULL)
        exit(2);
    else {
        mpz_set(output->base, currentNode->primeNumber);
        output->indexOfBaseRespectToFactorBase = index;
        mpz_set_ui(*(output->solution + index), 1);
    }

    return output;
}


void startSecondStep(DLogProblemInstance *instance) {

    instance->secondPhaseOutput = getBaseToComputeKnownLogarithm(instance);

    unsigned long moreRows = 75*instance->factorBase->length;
    unsigned long totalRow = instance->factorBase->length;

    Matrix *equationSystem = allocateMatrix(totalRow, instance->factorBase->length + 1);

    pthread_t *pthreads = startThreadPool(instance->threadsPoolSize, &threadRoutineForRelationRetrieval, (void *) instance->threadsPoolData);

    fprintf(stderr, "--> Producing relations...\n");

    for (unsigned long long currentRow = 0; currentRow != totalRow; currentRow++) {

        __mpz_struct **relation = popFromArrayOfCircularBufferRoundRobinManner(instance);

        for (unsigned long long currentColumn = 0; currentColumn < instance->factorBase->length; currentColumn++) {

            __mpz_struct *currentNumber = *(relation + currentColumn);

            if (instance->secondPhaseOutput->indexOfBaseRespectToFactorBase == currentColumn) {

                __mpz_struct *zero = allocateAndSetNumberFromULL(0);

                mpz_mul_si(currentNumber, currentNumber, -1);

                setNumberMatrixCell(equationSystem, currentRow, currentColumn, zero);
                setNumberMatrixCell(equationSystem, currentRow, instance->factorBase->length, currentNumber);

            } else {
                setNumberMatrixCell(equationSystem, currentRow, currentColumn, currentNumber);
            }
        }

        free(relation);
    }

    stopThreadsPool(instance);
    joinAndFreeThreadsPool(pthreads, instance->threadsPoolSize);

    fprintf(stderr, "--> Resolving equation system...\n");
    performGaussianElimination(equationSystem, instance->numbersBuffer, instance->discreteLogarithm->multiplicativeGroupMinusOne, instance->threadsPoolSize);

    populateSecondPhaseOutput(equationSystem, instance->secondPhaseOutput);

    freeMatrix(equationSystem);
}