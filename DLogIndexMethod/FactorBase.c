//
// Created by andrea on 21/11/19.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "FactorBase.h"
#include "../Math/Number.h"

FactorBaseNode *__allocateFactorBaseNode(__mpz_struct *primeNumber) {

    FactorBaseNode *output = malloc(sizeof(FactorBaseNode));
    if (output == NULL)
        exit(EXIT_FAILURE);

    output->primeNumber = primeNumber;
    output->next_node = NULL;

    return output;
}

void __insertIntoFactorBase(FactorBase *factorBase, __mpz_struct *primeNumber) {

    FactorBaseNode *newNode = __allocateFactorBaseNode(primeNumber);

    if (factorBase->length == 0) {

        factorBase->head = newNode;
        factorBase->tail = newNode;

    } else {

        ((FactorBaseNode *) factorBase->tail)->next_node = newNode;
        factorBase->tail = newNode;
    }

    factorBase->length++;
}

bool __isFactorBasePopulatedFromFileUntilSmoothnessBound(FactorBase *factorBase, __mpz_struct *smoothnessBound) {

    __mpz_struct *currentPrime;
    bool output = false;

    char *stringBuffer = malloc(sizeof(char) * 50);
    if (stringBuffer == NULL)
        exit(EXIT_FAILURE);

    FILE *primeNumberListFile = fopen("/home/andrea/PrimesList.txt", "r");
    if (primeNumberListFile == NULL)
        exit(EXIT_FAILURE);

    while (true) {

        int readData = fscanf(primeNumberListFile, "%s", stringBuffer);
        if (readData != EOF) {

            currentPrime = allocateAndSetNumberFromString(stringBuffer);

            if (mpz_cmp(currentPrime, smoothnessBound) > 0) {
                output = true;
                break;
            } else
                __insertIntoFactorBase(factorBase, currentPrime);

        } else {

            if (fclose(primeNumberListFile) == EOF)
                exit(EXIT_FAILURE);
            else
                break;
        }
    }

    free(stringBuffer);

    return output;
}

FactorBase *allocateFactorBase() {

    FactorBase *output = malloc(sizeof(FactorBase));
    if (output == NULL)
        exit(EXIT_FAILURE);

    output->head = NULL;
    output->length = 0;

    return output;
}

void populateFactorBase(FactorBase *factorBase, __mpz_struct *smoothnessBound) {

    bool isSmoothnessBoundReached = __isFactorBasePopulatedFromFileUntilSmoothnessBound(factorBase, smoothnessBound);

    if (isSmoothnessBoundReached)
        return;
    else {

        __mpz_struct *lastInsertedPrime = factorBase->tail->primeNumber;

        while (true) {

            __mpz_struct *currentPrime = allocateNumber();

            mpz_nextprime(currentPrime, lastInsertedPrime);

            if (mpz_cmp(currentPrime, smoothnessBound) > 0) {
                return;
            } else {
                __insertIntoFactorBase(factorBase, currentPrime);
                lastInsertedPrime = currentPrime;
            }
        }
    }
}