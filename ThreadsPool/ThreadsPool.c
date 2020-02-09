#include <pthread.h>
#include <stdlib.h>

pthread_t **startThreadPool(unsigned long long poolSize, void *(*thread_routine)(void *), void *thread_argument) {

    pthread_t **output = malloc(poolSize * sizeof(pthread_t*));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        for (unsigned long long index = 0; index < poolSize; index++) {
            *(output + index) = malloc(sizeof(pthread_t));
            if (*(output + index) == NULL)
                exit(EXIT_FAILURE);

            if (pthread_create(*(output + index), NULL, thread_routine, thread_argument) != 0)
                exit(EXIT_FAILURE);
        }

    return output;
}