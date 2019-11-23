//
// Created by andrea on 20/11/19.
//

#include <stdlib.h>
#include <pthread.h>
#include "SecondPhase.h"
#include "../Math/Number.h"
#include "../Math/Matrix.h"
#include "../Buffers/CircularBuffer.h"

SecondPhaseOutput *allocateSecondPhaseOutput(unsigned long long size) {

    SecondPhaseOutput *output = malloc(sizeof(SecondPhaseOutput));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->solution = allocateNumbersArray(size, true);
    }

    return output;
}

SecondPhaseOutput *
getSecondPhaseOutput(Matrix *resolvedEquationSystem, FactorBase *factorBase, __mpz_struct *chosenBase) {

    SecondPhaseOutput *output = allocateSecondPhaseOutput(factorBase->length);

    for (unsigned long long rowIndex = 0; rowIndex < resolvedEquationSystem->rowLength; rowIndex++) {

    }

    return output;
}


pthread_t *startThread(unsigned int parallelismDegree, void *(*thread_routine)(void *), void **thread_argument) {

    pthread_t *output = malloc(sizeof(pthread_t) * parallelismDegree);
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        for (unsigned int index = 0; index < parallelismDegree; index++)
            if (pthread_create((output + index), NULL, thread_routine, thread_argument) != 0)
                exit(EXIT_FAILURE);

    return output;
}

typedef struct {

    CircularBuffer *circularBuffer;



} SecondPhaseSupport;




void startSecondPhase(DLogProblemInstance *instance) {
    instance->secondPhaseOutput = allocateSecondPhaseOutput(instance->factorBase->length);
}