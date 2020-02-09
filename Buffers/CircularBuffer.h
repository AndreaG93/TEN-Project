#pragma once

#include <sys/types.h>

typedef struct {

    void **buffer;
    unsigned int head;
    unsigned int tail;
    unsigned int bufferSize;

    pthread_cond_t pthreadCondition;
    pthread_mutex_t pthreadMutex;

} CircularBuffer;

CircularBuffer *allocateCircularBuffer();

void deallocateCircularBuffer(CircularBuffer *buffer);

pthread_t **allocateAndStartThreadToClearCircular(CircularBuffer *input);

void pushIntoCircularBuffer(CircularBuffer *buffer, void *data);

void *popFromCircularBuffer(CircularBuffer *buffer);

void clearCircularBuffer(CircularBuffer *buffer);