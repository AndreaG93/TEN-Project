#include "DLogIndexMethod/DLogProblemInstance.h"
#include "DLogIndexMethod/ThirdPhase.h"
#include "DLogIndexMethod/FirstPhase.h"
#include "DLogIndexMethod/SecondPhase.h"
#include "DLogIndexMethod/RelationsRetrieval.h"
#include "ThreadsPool/ThreadsPool.h"

#define MAX_RANDOM_INTEGER 20

//#define EXECUTE_TESTS // Comment to disable tests...
#define AUDIT if(1)


int main() {

    DLogProblemInstance *instance = allocateDLogProblemInstance("179", "2", "13", MAX_RANDOM_INTEGER);
    setSmoothnessBound(instance, "7");


    instance->threadsPoolData = allocateThreadsPoolData(instance);

    startThreadPool(1, &threadRoutineForRelationRetrieval, (void *) instance->threadsPoolData);

    startFirstPhase(instance);
    startSecondPhase(instance);
    startThirdPhase(instance);

#ifdef EXECUTE_TESTS
    numberBuffer_test();
    getInverseMultiplicative_test();
    isInvertible_test();
#endif

    return 0;
}