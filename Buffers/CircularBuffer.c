//
// Created by andrea on 21/11/19.
//

#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <gmp.h>
#include "CircularBuffer.h"
#include "../ThreadsPool/ThreadsPool.h"


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

        output->buffer = malloc(output->bufferSize * sizeof(void *));
        if (output->buffer == NULL)
            exit(EXIT_FAILURE);
        else
            return output;
    }
}

void deallocateCircularBuffer(CircularBuffer *input) {

    while (input->head != input->tail) {

        __mpz_struct* currentElement = *(input->buffer + input->tail);

        free(currentElement);
        input->tail = (input->tail + 1) % input->bufferSize;
    }

    if (pthread_cond_destroy(&input->pthreadCondition) != 0)
        exit(EXIT_FAILURE);

    if (pthread_mutex_destroy(&input->pthreadMutex) != 0)
        exit(EXIT_FAILURE);

    free(input->buffer);
    free(input);
}

void clearCircularBuffer(CircularBuffer *buffer) {

    while (buffer->head != buffer->tail) {

        free(*(buffer->buffer + buffer->tail));
        buffer->tail = (buffer->tail + 1) % buffer->bufferSize;
    }
}

void *threadRoutineForCircularBufferCleaning(void *input) {

    clearCircularBuffer((CircularBuffer *) input);
    return NULL;
}

pthread_t **allocateAndStartThreadToClearCircular(CircularBuffer *input) {
    return startThreadPool(1, &threadRoutineForCircularBufferCleaning, input);
}

void pushIntoCircularBuffer(CircularBuffer *buffer, void *data) {

    pthread_mutex_lock(&buffer->pthreadMutex);

    if ((buffer->head + 1) % buffer->bufferSize == buffer->tail)
        pthread_cond_wait(&buffer->pthreadCondition, &buffer->pthreadMutex);


    *(buffer->buffer + buffer->head) = data;
    buffer->head = (buffer->head + 1) % buffer->bufferSize;

    pthread_cond_signal(&buffer->pthreadCondition);
    pthread_mutex_unlock(&buffer->pthreadMutex);


}

void *popFromCircularBuffer(CircularBuffer *buffer) {

    pthread_mutex_lock(&buffer->pthreadMutex);

    if (buffer->head == buffer->tail)
        pthread_cond_wait(&buffer->pthreadCondition, &buffer->pthreadMutex);


    __mpz_struct *output = *(buffer->buffer + buffer->tail);
    buffer->tail = (buffer->tail + 1) % buffer->bufferSize;

    pthread_cond_signal(&buffer->pthreadCondition);
    pthread_mutex_unlock(&buffer->pthreadMutex);

    return output;
}

