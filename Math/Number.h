#pragma once



__mpz_struct **allocateNumbersArray(unsigned long long size);

void deallocateNumbersArray(__mpz_struct **input, unsigned long long size);

__mpz_struct *allocateNumber();

__mpz_struct *allocateAndSetNumberFromString(char *number);

__mpz_struct *allocateAndSetNumberFromULL(unsigned long long number);

__mpz_struct *allocateAndSetNumberFromNumber(__mpz_struct *number);