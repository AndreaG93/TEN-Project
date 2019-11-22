#pragma once

#include <gmp.h>
#include "../Buffers/ApplicationBuffer.h"

typedef struct {
    __mpz_struct ***structure;

    unsigned long long rowLength;
    unsigned long long columnLength;
} Matrix;

Matrix *allocateMatrix(unsigned long long numberOfRow, unsigned long long numberOfColumn);

void setNumberIntoMatrixCell(Matrix *matrix, unsigned long long row, unsigned long long column, __mpz_struct *number);

__mpz_struct *getNumberFromMatrixCell(Matrix *matrix, unsigned long long row, unsigned long long column);

void performGaussianElimination(Matrix *matrix, ApplicationBuffer *buffer, __mpz_struct *modulo);

void printMatrix(Matrix *matrix);