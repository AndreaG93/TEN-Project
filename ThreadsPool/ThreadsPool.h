#pragma once

typedef struct {

    unsigned int threadID;
    void *threadArgument;

} ThreadArgument;

pthread_t *startThreadPool(unsigned long long poolSize, void *(*thread_routine)(void *), void *threadInputData);

void joinAndFreeThreadsPool(pthread_t *threadPoll, unsigned long long poolSize);