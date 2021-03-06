#pragma once
#include <stdbool.h>

__mpz_struct **allocateNumbersArray(unsigned long long size, bool isNumberAllocated);

void freeNumbersArray(__mpz_struct **input, unsigned long long size);

__mpz_struct *allocateNumber();

__mpz_struct *allocateAndSetNumberFromString(char *number);

__mpz_struct *allocateAndSetNumberFromULL(unsigned long long number);

__mpz_struct *allocateAndSetNumberFromNumber(__mpz_struct *number);

void freeNumber(__mpz_struct *input);

void printNumbersArray(__mpz_struct **relation, unsigned long size);