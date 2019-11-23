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

void pushCircularBuffer(CircularBuffer *buffer, void *data);

void *popCircularBuffer(CircularBuffer *buffer);

void freeCircularBuffer(CircularBuffer *buffer);