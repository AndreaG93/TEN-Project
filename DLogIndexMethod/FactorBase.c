//
// Created by andrea on 21/11/19.
//

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include "FactorBase.h"
#include "../Math/Number.h"

typedef struct {

    __mpz_struct **buffer;
    unsigned int head;
    unsigned int tail;
    unsigned int bufferSize;

    pthread_cond_t pthreadCondition;
    pthread_mutex_t pthreadMutex;

} PrimeNumberBuffer;


PrimeNumberBuffer *allocatePrimeNumberBuffer() {

    PrimeNumberBuffer *output = malloc(sizeof(FactorBaseNode));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {

        if (pthread_mutex_init(&output->pthreadMutex, NULL) != 0)
            exit(EXIT_FAILURE);

        if (pthread_cond_init(&output->pthreadCondition, NULL) != 0)
            exit(EXIT_FAILURE);

        output->head = 0;
        output->tail = 0;
        output->bufferSize = 1000;

        output->buffer = malloc(output->bufferSize * sizeof(__mpz_struct *));
        if (output->buffer == NULL)
            exit(EXIT_FAILURE);
        else
            return output;
    }
}

void primeNumberBufferPush(PrimeNumberBuffer *buffer, __mpz_struct *data) {

    pthread_mutex_lock(&buffer->pthreadMutex);

    if (((buffer->head + 1) % buffer->bufferSize) == buffer->tail)
        pthread_cond_wait(&buffer->pthreadCondition, &buffer->pthreadMutex);


    *(buffer->buffer + buffer->head) = data;
    buffer->head = (buffer->head + 1) % buffer->bufferSize;

    pthread_mutex_unlock(&buffer->pthreadMutex);
    pthread_cond_signal(&buffer->pthreadCondition);

}

__mpz_struct *primeNumberBufferPop(PrimeNumberBuffer *buffer, __mpz_struct *data) {

    pthread_mutex_lock(&buffer->pthreadMutex);

    if (buffer->head == buffer->tail)
        pthread_cond_wait(&buffer->pthreadCondition, &buffer->pthreadMutex);

    __mpz_struct *output = *(buffer->buffer + buffer->tail);
    buffer->tail = (buffer->tail + 1) % buffer->bufferSize;

    pthread_mutex_unlock(&buffer->pthreadMutex);
    pthread_cond_signal(&buffer->pthreadCondition);

    return output;
}


FactorBaseNode *allocateFactorBaseNode(__mpz_struct *primeNumber) {

    FactorBaseNode *output = malloc(sizeof(FactorBaseNode));
    if (output == NULL)
        exit(EXIT_FAILURE);


    output->primeNumber = primeNumber;
    output->next_node = NULL;

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

void insertIntoFactorBase(FactorBase *factorBase, __mpz_struct *primeNumber) {

    FactorBaseNode *newNode = allocateFactorBaseNode(primeNumber);

    if (factorBase->length == 0) {

        factorBase->head = newNode;
        factorBase->tail = newNode;

    } else {

        ((FactorBaseNode *) factorBase->tail)->next_node = newNode;
        factorBase->tail = newNode;
    }

    factorBase->length++;
}

bool isFactorBasePopulatedFromFileUntilSmoothnessBound(FactorBase *factorBase, __mpz_struct *smoothnessBound) {

    __mpz_struct *currentPrime;
    bool output = false;

    char *stringBuffer = malloc(sizeof(char) * 50);
    if (stringBuffer == NULL)
        exit(EXIT_FAILURE);

    FILE *primeNumberListFile = fopen("/Andrea/PrimesList.txt", "r");
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
                insertIntoFactorBase(factorBase, currentPrime);

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

void populateFactorBase(FactorBase *factorBase, __mpz_struct *smoothnessBound) {

    bool isSmoothnessBoundReached = isFactorBasePopulatedFromFileUntilSmoothnessBound(factorBase, smoothnessBound);

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
                insertIntoFactorBase(factorBase, currentPrime);
                lastInsertedPrime = currentPrime;
            }
        }
    }
}


