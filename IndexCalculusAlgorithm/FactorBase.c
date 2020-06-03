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

    FILE *primeNumberListFile = fopen("../PrimesList.txt", "r");
    if (primeNumberListFile == NULL)
        exit(EXIT_FAILURE);

    while (true) {

        int readData = fscanf(primeNumberListFile, "%s", stringBuffer);
        if (readData != EOF) {

            currentPrime = allocateAndSetNumberFromString(stringBuffer);

            if (mpz_cmp(currentPrime, smoothnessBound) > 0) {
                freeNumber(currentPrime);
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

void completeFactorBasePopulation(FactorBase* input) {

    input->arrayOfPrime = allocateNumbersArray(input->length, false);
    if (input->arrayOfPrime == NULL)
        exit(EXIT_FAILURE);

    unsigned int index = 0;
    for (FactorBaseNode *currentNode = input->head; currentNode != NULL; currentNode = currentNode->next_node, index++)
        *(input->arrayOfPrime + index) = currentNode->primeNumber;
}

void freeFactorBase(FactorBase* input) {

    FactorBaseNode *nextNode;

    for (FactorBaseNode *currentNode = input->head; currentNode != NULL; ) {

        nextNode = currentNode->next_node;

        freeNumber(currentNode->primeNumber);
        free(currentNode);

        currentNode = nextNode;
    }

    free(input->arrayOfPrime);
    free(input);
}

void populateFactorBase(FactorBase *factorBase, __mpz_struct *smoothnessBound) {

    __insertIntoFactorBase(factorBase, allocateAndSetNumberFromString("-1"));

    bool isSmoothnessBoundReached = __isFactorBasePopulatedFromFileUntilSmoothnessBound(factorBase, smoothnessBound);

    if (!isSmoothnessBoundReached) {

        __mpz_struct *lastInsertedPrime = factorBase->tail->primeNumber;

        while (true) {

            __mpz_struct *currentPrime = allocateNumber();

            mpz_nextprime(currentPrime, lastInsertedPrime);

            if (mpz_cmp(currentPrime, smoothnessBound) > 0) {
                break;
            } else {
                __insertIntoFactorBase(factorBase, currentPrime);
                lastInsertedPrime = currentPrime;
            }
        }
    }

    completeFactorBasePopulation(factorBase);
}