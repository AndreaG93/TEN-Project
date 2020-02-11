#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>

#include "Number.h"

__mpz_struct **allocateNumbersArray(unsigned long long size, bool isNumberAllocated) {

    __mpz_struct **output = malloc(size * sizeof(__mpz_struct *));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        if (isNumberAllocated)
            for (unsigned long long i = 0; i < size; i++)
                *(output + i) = allocateNumber();
        else
            for (unsigned long long i = 0; i < size; i++)
                *(output + i) = NULL;
    }

    return output;
}

void printNumbersArray(__mpz_struct **relation, unsigned long size) {

    for (int index = 0; index < size; index++)
        gmp_fprintf(stderr, " %Zd + ", *(relation + index));
    fprintf(stderr, "\n");
}

void freeNumber(__mpz_struct *input) {
    mpz_clear(input);
    free(input);
}

void freeNumbersArray(__mpz_struct **input, unsigned long long size) {

    for (unsigned long long i = 0; i < size; i++) {
        mpz_clear(*(input + i));
    }
    free(input);
}

__mpz_struct *allocateNumber() {
    __mpz_struct *output = malloc(sizeof(__mpz_struct));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        mpz_init(output);
        return output;
    }
}

__mpz_struct *allocateAndSetNumberFromULL(unsigned long long number) {

    __mpz_struct *output = malloc(sizeof(__mpz_struct));
    if (output == NULL)
        exit(EXIT_FAILURE);

    mpz_init_set_ui(output, number);
    return output;
}

__mpz_struct *allocateAndSetNumberFromString(char *number) {

    __mpz_struct *output = malloc(sizeof(__mpz_struct));
    if (output == NULL)
        exit(EXIT_FAILURE);

    if (mpz_init_set_str(output, number, 10) == 0)
        return output;
    else {
        mpz_clear(output);
        free(output);
        return NULL;
    }
}

__mpz_struct *allocateAndSetNumberFromNumber(__mpz_struct *number) {

    __mpz_struct *output = malloc(sizeof(__mpz_struct));
    if (output == NULL)
        exit(EXIT_FAILURE);

    mpz_init_set(output, number);
    return output;
}