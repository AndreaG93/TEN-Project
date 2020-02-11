#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"
#include "Number.h"
#include "Common.h"

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

void sumRows(Matrix *matrix, unsigned long long sourceRow, unsigned long long targetRow, __mpz_struct *multiplier,
             __mpz_struct *modulo, NumbersBuffer *numbersBuffer) {

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

void performGaussianElimination(Matrix *matrix, NumbersBuffer *numberBuffer, __mpz_struct *modulo) {

    unsigned long long actualTargetRow = 0;

    for (unsigned long long column = 0; column < matrix->columnLength; column++) {
        for (unsigned long long row = actualTargetRow; row < matrix->rowLength; row++) {

            __mpz_struct *currentElement = *(*(matrix->structure + column) + row);

            if (mpz_cmp_ui(currentElement, 0) != 0 && isInvertible(numberBuffer, currentElement, modulo)) {
                swapRows(matrix, row, actualTargetRow);

                __mpz_struct *inverseOfCurrentElement = allocateNumber();
                mpz_invert(inverseOfCurrentElement, currentElement, modulo);
                multiplyRowByScalar(matrix, actualTargetRow, inverseOfCurrentElement, modulo);

                mpz_clear(inverseOfCurrentElement);
                free(inverseOfCurrentElement);

                for (unsigned long long subRow = 0; subRow < matrix->rowLength; subRow++) {

                    if (subRow != actualTargetRow) {

                        __mpz_struct *multiplier = allocateNumber();
                        mpz_set(multiplier, *(*(matrix->structure + column) + subRow));
                        mpz_mul_si(multiplier, multiplier, -1);

                        sumRows(matrix, actualTargetRow, subRow, multiplier, modulo, numberBuffer);

                        mpz_clear(multiplier);
                        free(multiplier);
                    }
                }
                actualTargetRow++;
            }
        }
    }
}

#ifdef DEBUG

void printMatrix(Matrix *matrix) {

    for (unsigned long long rowIndex = 0; rowIndex < matrix->rowLength; rowIndex++) {

        for (unsigned long long columnIndex = 0; columnIndex < matrix->columnLength; columnIndex++) {
            __mpz_struct* number = *(*(matrix->structure + columnIndex) + rowIndex);
            gmp_printf("%Zd ", number);
        }


        fprintf(stdout, "\n");
    }
}

#endif