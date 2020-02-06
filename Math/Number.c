#include <gmp.h>
#include <stdlib.h>

#include "Number.h"

__mpz_struct **allocateNumbersArray(unsigned long long size, bool isNumberAllocated) {

    __mpz_struct **output = malloc(size * sizeof(__mpz_struct *));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        if (isNumberAllocated)
            for (unsigned long long i = 0; i < size; i++)
                *(output + i) = allocateNumber();
    }

    return output;
}
void deallocateNumber(__mpz_struct *input) {
    mpz_clear(input);
    free(input);
}

void deallocateNumbersArray(__mpz_struct **input, unsigned long long size) {

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

    mpz_init_set_str(output, number, 10);
    return output;
}

__mpz_struct *allocateAndSetNumberFromNumber(__mpz_struct *number) {

    __mpz_struct *output = malloc(sizeof(__mpz_struct));
    if (output == NULL)
        exit(EXIT_FAILURE);

    mpz_init_set(output, number);
    return output;
}