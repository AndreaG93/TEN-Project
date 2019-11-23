//
// Created by andrea on 21/11/19.
//

#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <gmp.h>
#include <stdio.h>
#include "CircularBuffer.h"


CircularBuffer *allocateCircularBuffer() {

    CircularBuffer *output = malloc(sizeof(CircularBuffer));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {

        if (pthread_mutex_init(&(output->pthreadMutex), NULL) != 0)
            exit(EXIT_FAILURE);

        if (pthread_cond_init(&(output->pthreadCondition), NULL) != 0)
            exit(EXIT_FAILURE);

        output->head = 0;
        output->tail = 0;
        output->bufferSize = 10000;

        output->buffer = calloc(output->bufferSize, sizeof(void *));
        if (output->buffer == NULL)
            exit(EXIT_FAILURE);
        else
            return output;
    }
}

void pushCircularBuffer(CircularBuffer *buffer, void *data) {

    pthread_mutex_lock(&buffer->pthreadMutex);

    if ((buffer->head + 1) % buffer->bufferSize == buffer->tail)
        pthread_cond_wait(&buffer->pthreadCondition, &buffer->pthreadMutex);


    *(buffer->buffer + buffer->head) = data;
    buffer->head = (buffer->head + 1) % buffer->bufferSize;

    pthread_cond_signal(&buffer->pthreadCondition);
    pthread_mutex_unlock(&buffer->pthreadMutex);


}

void *popCircularBuffer(CircularBuffer *buffer) {

    pthread_mutex_lock(&buffer->pthreadMutex);

    if (buffer->head == buffer->tail)
        pthread_cond_wait(&buffer->pthreadCondition, &buffer->pthreadMutex);


    __mpz_struct *output = *(buffer->buffer + buffer->tail);
    buffer->tail = (buffer->tail + 1) % buffer->bufferSize;

    pthread_cond_signal(&buffer->pthreadCondition);
    pthread_mutex_unlock(&buffer->pthreadMutex);

    return output;
}

void freeCircularBuffer(CircularBuffer *buffer) {

    while (buffer->head != buffer->tail) {

        __mpz_struct* currentElement = *(buffer->buffer + buffer->tail);

        free(currentElement);
        buffer->tail = (buffer->tail + 1) % buffer->bufferSize;
    }

    if (pthread_cond_destroy(&buffer->pthreadCondition) != 0)
        exit(EXIT_FAILURE);

    if (pthread_mutex_destroy(&buffer->pthreadMutex) != 0)
        exit(EXIT_FAILURE);

    free(buffer->buffer);
    free(buffer);
}