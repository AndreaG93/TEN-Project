#include <pthread.h>
#include <stdlib.h>
#include "ThreadsPool.h"

ThreadArgument *allocateThreadArgument(unsigned int threadID, void *threadArgument) {

    ThreadArgument *output = malloc(sizeof(ThreadArgument));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
    {
        output->threadArgument = threadArgument;
        output->threadID = threadID;
    }

    return output;
}

pthread_t *startThreadPool(unsigned long long poolSize, void *(*thread_routine)(void *), void *threadInputData) {

    pthread_t *output = malloc(poolSize * sizeof(pthread_t));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        for (unsigned long long index = 0; index < poolSize; index++) {

            ThreadArgument* threadArgument = allocateThreadArgument(index, threadInputData);

            if (pthread_create(&output[index], NULL, thread_routine, threadArgument) != 0)
                exit(EXIT_FAILURE);
        }

    return output;
}

void joinAndFreeThreadsPool(pthread_t *threadPoll, unsigned long long poolSize) {

    for (unsigned long long index = 0; index < poolSize; index++) {
        pthread_join(threadPoll[index], NULL);

    }
    free(threadPoll);
}