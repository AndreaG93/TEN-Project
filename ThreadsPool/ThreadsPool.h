#pragma once

#include <stdbool.h>
#include "../Buffers/CircularBuffer.h"

typedef struct {

    CircularBuffer *buffer;
    bool stoppingCondition;

    void *threadData;

} ThreadsPoolData;

ThreadsPoolData *allocateThreadsPoolData(void *threadData);

pthread_t *startThreadsPool(unsigned long long poolSize, void *(*thread_routine)(void *), void *thread_argument);