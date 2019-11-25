//
// Created by andrea on 23/11/19.
//

#include <pthread.h>
#include <stdlib.h>
#include "ThreadsPool.h"

ThreadsPoolData *allocateThreadsPoolData(void *threadData) {

    ThreadsPoolData *output = malloc(sizeof(ThreadsPoolData));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->buffer = allocateCircularBuffer();
        output->stoppingCondition = false;
        output->pauseCondition = true;
        output->threadData = threadData;

        if (pthread_mutex_init(&(output->pthreadMutex), NULL) != 0)
            exit(EXIT_FAILURE);

        if (pthread_cond_init(&(output->pthreadCondition), NULL) != 0)
            exit(EXIT_FAILURE);
    }

    return output;
}

pthread_t *startThreadsPool(unsigned long long poolSize, void *(*thread_routine)(void *), void *thread_argument) {

    pthread_t *output = malloc(sizeof(pthread_t) * poolSize);
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        for (unsigned long long index = 0; index < poolSize; index++)
            if (pthread_create((output + index), NULL, thread_routine, thread_argument) != 0)
                exit(EXIT_FAILURE);

    return output;
}