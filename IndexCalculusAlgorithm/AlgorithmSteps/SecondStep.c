//
// Created by andrea on 20/11/19.
//

#include <stdlib.h>
#include <pthread.h>
#include "SecondStep.h"
#include "../../Math/Number.h"
#include "../../Math/Matrix.h"

SecondPhaseOutput *allocateSecondPhaseOutput(unsigned long long size) {

    SecondPhaseOutput *output = malloc(sizeof(SecondPhaseOutput));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->solution = allocateNumbersArray(size, true);
    }

    return output;
}

SecondPhaseOutput *populateSecondPhaseOutput(Matrix *resolvedEquationSystem, FactorBase *factorBase, unsigned long long special) {

    SecondPhaseOutput *output = allocateSecondPhaseOutput(factorBase->length);

    bool found = true;
    unsigned long long columnIndex = resolvedEquationSystem->columnLength - 1;
    unsigned long long outputIndex = 0;
    for (unsigned long long rowIndex = 0; rowIndex < resolvedEquationSystem->rowLength; rowIndex++) {

        __mpz_struct *number;

        if (special == rowIndex && found) {
            number = allocateAndSetNumberFromULL(1);
            rowIndex--;
            found = false;
        } else {
            number = getNumberMatrixCell(resolvedEquationSystem, rowIndex, columnIndex);
        }

        *(output->solution + outputIndex) = number;
        outputIndex++;
    }

    return output;
}

unsigned long long found(FactorBase *factorBase, __mpz_struct *chosenBase) {

    unsigned long long output = 0;
    FactorBaseNode *currentNode = factorBase->head;

    while (currentNode != NULL) {

        if (mpz_cmp(currentNode->primeNumber, chosenBase) == 0)
            break;
        else {

            output++;
            currentNode = currentNode->next_node;
        }
    }

    return output;
}


void startSecondStep(DLogProblemInstance *instance) {

    sendSignalToThreadsPoolToExecuteSpecifiedAlgorithmStep(instance, 2);

    Matrix *equationSystem = allocateMatrix(instance->factorBase->length + 4, instance->factorBase->length + 1);

    unsigned long long indexFFF = found(instance->factorBase, instance->discreteLogarithm->base);
    unsigned long long currentRow = 0;

    while (currentRow != instance->factorBase->length + 17) {

        __mpz_struct **relation = popFromCircularBuffer(instance->threadsPoolData->sharedBuffer);

        for (unsigned long long currentColumn = 0; currentColumn < instance->factorBase->length; currentColumn++) {

            __mpz_struct *currentNumber = *(relation + currentColumn);

            if (indexFFF == currentColumn) {

                __mpz_struct *zero = allocateAndSetNumberFromULL(0);

                mpz_mul_si(currentNumber, currentNumber, -1);

                setNumberMatrixCell(equationSystem, currentRow, currentColumn, zero);
                setNumberMatrixCell(equationSystem, currentRow, instance->factorBase->length, currentNumber);

            } else {
                setNumberMatrixCell(equationSystem, currentRow, currentColumn, currentNumber);
            }
        }

        currentRow++;
    }

    pauseThreadsPool(instance);
    printMatrix(equationSystem);
    pthread_t *cleanerThread = allocateAndStartThreadToClearCircular(instance->threadsPoolData->sharedBuffer);

    performGaussianElimination(equationSystem, instance->numbersBuffer, instance->discreteLogarithm->multiplicativeGroupMinusOne);
    printMatrix(equationSystem);
    instance->secondPhaseOutput = populateSecondPhaseOutput(equationSystem, instance->factorBase, indexFFF);

    pthread_join(*cleanerThread, NULL);
    free(cleanerThread);
}

