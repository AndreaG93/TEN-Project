//
// Created by andrea on 20/11/19.
//

#include <stdlib.h>
#include <pthread.h>
#include "SecondStep.h"
#include "../../Math/Number.h"
#include "../../Math/Matrix.h"
#include "../../Math/Common.h"

#define FURTHER_RELATIONS 17

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

__mpz_struct **extractSolutionWithDelta(DLogProblemInstance *instance, Matrix *matrix) {

    __mpz_struct **output = allocateNumbersArray(instance->factorBase->length, true);
    int currentIndex = 0;

    bool oneDominantFound = false;
    bool isNumberSet = false;

    for (unsigned long long currentRow = 0; currentRow < instance->factorBase->length; currentRow++) {
        for (unsigned long long currentColumn = 0; currentColumn < instance->factorBase->length; currentColumn++) {

            __mpz_struct *currentNumber = getNumberMatrixCell(matrix, currentRow, currentColumn);

            if (mpz_cmp_ui(currentNumber, 0) == 0)
                continue;
            else if (mpz_cmp_ui(currentNumber, 1) == 0 && !oneDominantFound)
                oneDominantFound = true;
            else if ((mpz_cmp_ui(currentNumber, 1) == 0 && oneDominantFound) || mpz_cmp_ui(currentNumber, 1) != 0) {
                mpz_set(*(output + currentIndex), currentNumber);
                isNumberSet = true;
                break;
            }
        }

        if (!isNumberSet) {
            mpz_set_ui(*(output + currentIndex), 0);
        }
        currentIndex++;

        oneDominantFound = false;
        isNumberSet = false;
    }

    for (int index = 0; index < instance->factorBase->length; index++)
        gmp_fprintf(stderr, " %Zd + ", *(output + index));
    fprintf(stderr, "\n");

    return output;
}

SecondPhaseOutput *extractSolution(__mpz_struct** solutionWithDelta, DLogProblemInstance *instance) {

    SecondPhaseOutput *output = allocateSecondPhaseOutput(instance->factorBase->length);

    __mpz_struct* delta = retrieveNumberFromBuffer(instance->numbersBuffer);









    releaseNumber(instance->numbersBuffer);

    return output;
}

SecondPhaseOutput* getBaseToComputeKnownLogarithm(DLogProblemInstance *instance) {

    SecondPhaseOutput *output = allocateSecondPhaseOutput(instance->factorBase->length);

    unsigned long index = 0;

    for (FactorBaseNode *currentNode = instance->factorBase->head; currentNode != NULL; currentNode = currentNode->next_node, output++) {

        if (mpz_cmp(currentNode->primeNumber, instance->discreteLogarithm->base) == 0) {

            mpz_set(output->base, currentNode->primeNumber);
            output->indexOfBaseRespectToFactorBase = index;
            break;

        } else {

            if(isGroupGenerator(currentNode->primeNumber, instance->discreteLogarithm->multiplicativeGroup, instance->numbersBuffer, instance->randomIntegerGenerator)){
                mpz_set(output->base, currentNode->primeNumber);
                output->indexOfBaseRespectToFactorBase = index;
                break;
            }
        }
    }

    return output;
}


void startSecondStep(DLogProblemInstance *instance) {

    SecondPhaseOutput* output = getBaseToComputeKnownLogarithm(instance);

    sendSignalToThreadsPoolToExecuteSpecifiedAlgorithmStep(instance, 2);

    Matrix *equationSystem = allocateMatrix(instance->factorBase->length + FURTHER_RELATIONS, instance->factorBase->length + 1);

    for (unsigned long long currentRow = 0; currentRow < instance->factorBase->length + FURTHER_RELATIONS; currentRow++) {

        __mpz_struct **relation = popFromCircularBuffer(instance->threadsPoolData->sharedBuffer);

        for (unsigned long long currentColumn = 0; currentColumn < instance->factorBase->length; currentColumn++) {

            __mpz_struct *currentNumber = *(relation + currentColumn);
            setNumberMatrixCell(equationSystem, currentRow, currentColumn, currentNumber);
        }

        setNumberMatrixCell(equationSystem, currentRow, instance->factorBase->length, allocateAndSetNumberFromULL(0));
    }

    pauseThreadsPool(instance);
    pthread_t *cleanerThread = allocateAndStartThreadToClearCircular(instance->threadsPoolData->sharedBuffer);

    performGaussianElimination(equationSystem, instance->numbersBuffer, instance->discreteLogarithm->multiplicativeGroupMinusOne);
    printMatrix(equationSystem);
    //__mpz_struct** solutionWithDelta = extractSolutionWithDelta(instance, equationSystem);

    instance->secondPhaseOutput = populateSecondPhaseOutput(equationSystem, instance->factorBase, 1);

    pthread_join(*cleanerThread, NULL);
    free(cleanerThread);
}

