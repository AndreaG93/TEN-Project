//
// Created by andrea on 20/11/19.
//

#include <stdlib.h>
#include <pthread.h>
#include "SecondPhase.h"
#include "../Math/Number.h"
#include "../Math/Matrix.h"
#include "../Buffers/CircularBuffer.h"

SecondPhaseOutput *allocateSecondPhaseOutput(unsigned long long size) {

    SecondPhaseOutput *output = malloc(sizeof(SecondPhaseOutput));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->solution = allocateNumbersArray(size, true);
    }

    return output;
}

SecondPhaseOutput *
populateSecondPhaseOutput(Matrix *resolvedEquationSystem, FactorBase *factorBase, unsigned long long special) {

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
            number = getNumberFromMatrixCell(resolvedEquationSystem, rowIndex, columnIndex);
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


void startSecondPhase(DLogProblemInstance *instance) {

    instance->currentPhase = 2;
    instance->threadsPoolData->pauseCondition = false;

    pthread_cond_signal(&instance->threadsPoolData->pthreadCondition);

    Matrix *equationSystem = allocateMatrix(instance->factorBase->length + 15, instance->factorBase->length + 1);

    unsigned long long indexFFF = found(instance->factorBase, instance->discreteLogarithmToCompute->base);
    unsigned long long currentRow = 0;

    while (currentRow != instance->factorBase->length + 15) {

        __mpz_struct **relation = popCircularBuffer(instance->threadsPoolData->buffer);

        for (unsigned long long currentColumn = 0; currentColumn < instance->factorBase->length; currentColumn++) {

            __mpz_struct *currentNumber = *(relation + currentColumn);


            if (indexFFF == currentColumn) {

                __mpz_struct *zero = allocateAndSetNumberFromULL(0);

                mpz_mul_si(currentNumber, currentNumber, -1);

                setNumberIntoMatrixCell(equationSystem, currentRow, currentColumn, zero);
                setNumberIntoMatrixCell(equationSystem, currentRow, instance->factorBase->length, currentNumber);

            } else {

                setNumberIntoMatrixCell(equationSystem, currentRow, currentColumn, currentNumber);
            }
        }

        currentRow++;
    }

    instance->threadsPoolData->pauseCondition = true;

    performGaussianElimination(equationSystem, instance->applicationBuffer,
                               instance->moduloOfMultiplicativeGroupMinusOne);

    clearCircularBuffer(instance->threadsPoolData->buffer);

    instance->secondPhaseOutput = populateSecondPhaseOutput(equationSystem, instance->factorBase, indexFFF);


    for (unsigned long index = 0; index < instance->factorBase->length; index++) {
        gmp_printf("%Zd \n", *(instance->secondPhaseOutput->solution + index));
    }

}