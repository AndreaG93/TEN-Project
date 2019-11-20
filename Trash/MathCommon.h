#pragma once

#include <gmp.h>
#include <stdbool.h>
#include <string.h>
#include "DataStructures/UnorderedList.h"
#include "../Math/Number.h"

void generatePrimeDatabase() {

    char *currentPrimeStr;
    size_t numOfDigits;

    FILE *outputFile = fopen("/Andrea/PrimeDatabase.txt", "w+");
    if (outputFile == NULL)
        exit(EXIT_FAILURE);

    __mpz_struct *currentPrime = allocateNumber();

    mpz_set_ui(currentPrime, 999999999989);

    while (true) {
        mpz_nextprime(currentPrime, currentPrime);

        currentPrimeStr = mpz_get_str(NULL, 10, currentPrime);
        numOfDigits = strlen(currentPrimeStr);
        fprintf(outputFile, "%s\n", currentPrimeStr);

        if (numOfDigits == 15) {
            break;
        }
    }

    if (fclose(outputFile) != 0) {
        exit(EXIT_FAILURE);
    }
}