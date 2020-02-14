#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Matrix.h"
#include "Number.h"
#include "Common.h"
#include "../ThreadsPool/ThreadsPool.h"

Matrix *allocateMatrix(unsigned long long numberOfRow, unsigned long long numberOfColumn) {

    Matrix *output = malloc(sizeof(Matrix));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {

        output->columnLength = numberOfColumn;
        output->rowLength = numberOfRow;

        output->structure = malloc(numberOfColumn * sizeof(__mpz_struct **));
        if (output->structure == NULL)
            exit(EXIT_FAILURE);
        else
            for (unsigned long long columnIndex = 0; columnIndex < output->columnLength; columnIndex++)
                *(output->structure + columnIndex) = allocateNumbersArray(numberOfRow, false);
    }

    return output;
}

void freeMatrix(Matrix *matrix) {

    for (unsigned long long columnIndex = 0; columnIndex < matrix->columnLength; columnIndex++) {

        __mpz_struct **currentColumn = *(matrix->structure + columnIndex);

        for (unsigned long long rowIndex = 0; rowIndex < matrix->rowLength; rowIndex++) {

            __mpz_struct *currentNumber = *(currentColumn + rowIndex);

            if (currentNumber != NULL)
                freeNumber(currentNumber);
        }

        free(currentColumn);
    }

    free(matrix->structure);
    free(matrix);
}

void setNumberMatrixCell(Matrix *matrix, unsigned long long rowIndex, unsigned long long columnIndex, __mpz_struct *number) {
    *(*(matrix->structure + columnIndex) + rowIndex) = number;
}

__mpz_struct *getNumberMatrixCell(Matrix *matrix, unsigned long long rowIndex, unsigned long long columnIndex) {
    return *(*(matrix->structure + columnIndex) + rowIndex);
}

void swapRows(Matrix *matrix, unsigned long long row1, unsigned long long row2) {

    for (unsigned long long column = 0; column < matrix->columnLength; column++) {

        __mpz_struct *firstElement = *(*(matrix->structure + column) + row1);
        __mpz_struct *secondElement = *(*(matrix->structure + column) + row2);

        *(*(matrix->structure + column) + row2) = firstElement;
        *(*(matrix->structure + column) + row1) = secondElement;
    }
}

void multiplyRowByScalar(Matrix *matrix, unsigned long long rowIndex, __mpz_struct *scalar, __mpz_struct *modulo) {

    for (unsigned long long column = 0; column < matrix->columnLength; column++) {

        __mpz_struct *element = *(*(matrix->structure + column) + rowIndex);

        mpz_mul(element, element, scalar);
        mpz_mod(element, element, modulo);
    }
}

void sumRows(Matrix *matrix, unsigned long long sourceRow, unsigned long long targetRow, __mpz_struct *multiplier, __mpz_struct *modulo, NumbersBuffer *numbersBuffer) {

    __mpz_struct *aux = retrieveNumberFromBuffer(numbersBuffer);

    for (unsigned long long column = 0; column < matrix->columnLength; column++) {

        __mpz_struct *sourceElement = *(*(matrix->structure + column) + sourceRow);
        __mpz_struct *targetElement = *(*(matrix->structure + column) + targetRow);

        mpz_mul(aux, sourceElement, multiplier);
        mpz_add(targetElement, targetElement, aux);
        mpz_mod(targetElement, targetElement, modulo);
    }

    releaseNumber(numbersBuffer);
}

typedef struct {

    Matrix *matrix;
    NumbersBuffer **numbersBuffer;
    __mpz_struct *modulo;
    pthread_barrier_t firstBarrier;
    pthread_barrier_t secondBarrier;

    unsigned int *firstRowToManage;
    unsigned int *lastRowToManage;

    unsigned int actualTargetRow;
    unsigned int column;

    bool exit;

} ThreadMatrixArgument;

ThreadMatrixArgument *allocateThreadMatrixArgument(Matrix *matrix, __mpz_struct *modulo, unsigned int threadPoolSize) {

    ThreadMatrixArgument *output = malloc(sizeof(ThreadMatrixArgument));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->matrix = matrix;
        output->modulo = modulo;
        pthread_barrier_init(&output->firstBarrier,NULL,threadPoolSize + 1);
        pthread_barrier_init(&output->secondBarrier,NULL,threadPoolSize + 1);

        output->numbersBuffer = malloc(threadPoolSize * sizeof(NumbersBuffer *));
        if (output->numbersBuffer == NULL)
            exit(EXIT_FAILURE);

        for (int threadID = 0; threadID < threadPoolSize; threadID++)
            output->numbersBuffer[threadID] = allocateNumbersBuffer(2);

        output->firstRowToManage = malloc(threadPoolSize * sizeof(unsigned int));
        if (output->firstRowToManage == NULL)
            exit(EXIT_FAILURE);

        output->lastRowToManage = malloc(threadPoolSize * sizeof(unsigned int));
        if (output->lastRowToManage == NULL)
            exit(EXIT_FAILURE);

        output->exit = false;
    }

    return output;
}

void freeThreadMatrixArgument(ThreadMatrixArgument *input, unsigned int threadPoolSize) {
    free(input->firstRowToManage);
    free(input->lastRowToManage);

    for (int threadID = 0; threadID < threadPoolSize; threadID++)
        freeNumbersBuffer(input->numbersBuffer[threadID]);

    free(input->numbersBuffer);
    pthread_barrier_destroy(&input->firstBarrier);
    pthread_barrier_destroy(&input->secondBarrier);
    free(input);
}

void *threadRoutineSubRowManagement(void *input) {

    ThreadArgument *threadArgument = (ThreadArgument *) input;
    ThreadMatrixArgument *threadMatrixArgument = (ThreadMatrixArgument *) threadArgument->threadArgument;

    Matrix *matrix = threadMatrixArgument->matrix;
    __mpz_struct *modulo = threadMatrixArgument->modulo;
    NumbersBuffer *numbersBuffer = threadMatrixArgument->numbersBuffer[threadArgument->threadID];

    while (true) {

        pthread_barrier_wait(&threadMatrixArgument->firstBarrier);
        if (threadMatrixArgument->exit == true)
            break;

        for (unsigned long long subRow = threadMatrixArgument->firstRowToManage[threadArgument->threadID]; subRow < threadMatrixArgument->lastRowToManage[threadArgument->threadID]; subRow++) {

            if (subRow != threadMatrixArgument->actualTargetRow) {

                __mpz_struct *multiplier = retrieveNumberFromBuffer(numbersBuffer);
                mpz_set(multiplier, *(*(matrix->structure + threadMatrixArgument->column) + subRow));
                mpz_mul_si(multiplier, multiplier, -1);

                sumRows(matrix, threadMatrixArgument->actualTargetRow, subRow, multiplier, modulo, numbersBuffer);

                releaseNumber(numbersBuffer);
            }
        }

        pthread_barrier_wait(&threadMatrixArgument->secondBarrier);
    }

    free(threadArgument);

    return NULL;
}


void performGaussianElimination(Matrix *matrix, NumbersBuffer *numbersBuffer, __mpz_struct *modulo, unsigned int poolThreadSize) {

    ThreadMatrixArgument *threadMatrixArgument = allocateThreadMatrixArgument(matrix, modulo, poolThreadSize);
    pthread_t *pthreads = startThreadPool(poolThreadSize, &threadRoutineSubRowManagement, (void *) threadMatrixArgument);

    unsigned long long numberOfRowsPerThread = matrix->rowLength / poolThreadSize;
    unsigned long long actualTargetRow = 0;

    for (unsigned long long column = 0; column < matrix->columnLength; column++) {
        for (unsigned long long row = actualTargetRow; row < matrix->rowLength; row++) {

            __mpz_struct *currentElement = *(*(matrix->structure + column) + row);

            if (mpz_cmp_ui(currentElement, 0) != 0 && isInvertible(numbersBuffer, currentElement, modulo)) {
                swapRows(matrix, row, actualTargetRow);

                __mpz_struct *inverseOfCurrentElement = allocateNumber();
                mpz_invert(inverseOfCurrentElement, currentElement, modulo);
                multiplyRowByScalar(matrix, actualTargetRow, inverseOfCurrentElement, modulo);

                mpz_clear(inverseOfCurrentElement);
                free(inverseOfCurrentElement);

                threadMatrixArgument->column = column;
                threadMatrixArgument->actualTargetRow = actualTargetRow;

                unsigned lastAssignedRow = 0;
                for (int threadID = 0; threadID < poolThreadSize - 1; threadID++) {
                    threadMatrixArgument->firstRowToManage[threadID] = lastAssignedRow;
                    threadMatrixArgument->lastRowToManage[threadID] = lastAssignedRow + numberOfRowsPerThread;

                    lastAssignedRow = lastAssignedRow + numberOfRowsPerThread;
                }
                threadMatrixArgument->firstRowToManage[poolThreadSize - 1] = lastAssignedRow;
                threadMatrixArgument->lastRowToManage[poolThreadSize - 1] = matrix->rowLength;

                pthread_barrier_wait(&threadMatrixArgument->firstBarrier);
                pthread_barrier_wait(&threadMatrixArgument->secondBarrier);

                actualTargetRow++;
            }
        }
    }

    threadMatrixArgument->exit = true;

    pthread_barrier_wait(&threadMatrixArgument->firstBarrier);
    joinAndFreeThreadsPool(pthreads, poolThreadSize);
    freeThreadMatrixArgument(threadMatrixArgument, poolThreadSize);
}

void printMatrix(Matrix *matrix) {

    for (unsigned long long rowIndex = 0; rowIndex < matrix->rowLength; rowIndex++) {

        for (unsigned long long columnIndex = 0; columnIndex < matrix->columnLength; columnIndex++) {
            __mpz_struct *number = *(*(matrix->structure + columnIndex) + rowIndex);
            gmp_printf("%Zd ", number);
        }


        fprintf(stdout, "\n");
    }
}
