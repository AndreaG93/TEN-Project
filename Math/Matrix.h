#pragma once

#include <gmp.h>
#include "../Buffers/NumbersBuffer.h"
#define DEBUG

typedef struct {
    __mpz_struct ***structure;

    unsigned long long rowLength;
    unsigned long long columnLength;
} Matrix;

Matrix *allocateMatrix(unsigned long long numberOfRow, unsigned long long numberOfColumn);

void deallocateMatrix(Matrix *matrix);

pthread_t *allocateAndStartThreadToDeallocateMatrix(Matrix *input);

void
setNumberMatrixCell(Matrix *matrix, unsigned long long rowIndex, unsigned long long columnIndex, __mpz_struct *number);

__mpz_struct *getNumberMatrixCell(Matrix *matrix, unsigned long long rowIndex, unsigned long long columnIndex);

void performGaussianElimination(Matrix *matrix, NumbersBuffer *buffer, __mpz_struct *modulo);

#ifdef DEBUG
void printMatrix(Matrix *matrix);
#endif
