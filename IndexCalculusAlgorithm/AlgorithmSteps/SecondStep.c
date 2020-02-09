//
// Created by andrea on 20/11/19.
//

#include <stdlib.h>
#include <pthread.h>
#include "SecondStep.h"
#include "../../Math/Number.h"
#include "../../Math/Matrix.h"
#include "../../Math/Common.h"
#include "../RelationsRetrieval.h"
#include "../../ThreadsPool/ThreadsPool.h"

#define FURTHER_RELATIONS 6000
#define POOL_SIZE 4

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

    unsigned long index = 0;

    for (FactorBaseNode *currentNode = instance->factorBase->head; currentNode != NULL; currentNode = currentNode->next_node, index++) {
        if (isGroupGenerator(currentNode->primeNumber, instance->discreteLogarithm->multiplicativeGroup, instance->numbersBuffer, instance->randomIntegerGenerator)) {
            mpz_set(output->base, currentNode->primeNumber);
            output->indexOfBaseRespectToFactorBase = index;
            mpz_set_ui(*(output->solution + index), 1);
            break;
        }
    }

    return output;
}


void startSecondStep(DLogProblemInstance *instance) {

    instance->currentIndexCalculusAlgorithmStep = 2;
    instance->secondPhaseOutput = getBaseToComputeKnownLogarithm(instance);

    Matrix *equationSystem = allocateMatrix(instance->factorBase->length + FURTHER_RELATIONS, instance->factorBase->length + 1);

    pthread_t **pthreads = startThreadPool(POOL_SIZE, &threadRoutineForRelationRetrieval, (void *) instance->threadsPoolData);

    for (unsigned long long currentRow = 0; currentRow != instance->factorBase->length + FURTHER_RELATIONS; currentRow++) {

        __mpz_struct **relation = popFromCircularBuffer(instance->threadsPoolData->sharedBuffer);

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
    }

    stopThreadsPool(instance);

    for (unsigned long long index = 0; index < POOL_SIZE; index++)
        pthread_join(**(pthreads + index), NULL);

    pthread_t **cleanerThread = allocateAndStartThreadToClearCircular(instance->threadsPoolData->sharedBuffer);

    performGaussianElimination(equationSystem, instance->numbersBuffer, instance->discreteLogarithm->multiplicativeGroupMinusOne);
    populateSecondPhaseOutput(equationSystem, instance->secondPhaseOutput);

    pthread_join(**cleanerThread, NULL);
    free(cleanerThread);
}