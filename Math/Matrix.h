#pragma once

#include <gmp.h>
#include "../Buffers/NumbersBuffer.h"

typedef struct {
    __mpz_struct ***structure;

    unsigned long long rowLength;
    unsigned long long columnLength;
} Matrix;

Matrix *allocateMatrix(unsigned long long numberOfRow, unsigned long long numberOfColumn);

void freeMatrix(Matrix *matrix);

void setNumberMatrixCell(Matrix *matrix, unsigned long long rowIndex, unsigned long long columnIndex, __mpz_struct *number);

__mpz_struct *getNumberMatrixCell(Matrix *matrix, unsigned long long rowIndex, unsigned long long columnIndex);

void performGaussianElimination(Matrix *matrix, NumbersBuffer *numbersBuffer, __mpz_struct *modulo, unsigned int poolThreadSize);

#ifdef DEBUG

void printMatrix(Matrix *matrix);

#endif
