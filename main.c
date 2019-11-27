#include "DLogIndexMethod/DLogProblemInstance.h"
#include "DLogIndexMethod/ThirdPhase.h"
#include "DLogIndexMethod/FirstPhase.h"
#include "DLogIndexMethod/SecondPhase.h"
#include "DLogIndexMethod/RelationsRetrieval.h"
#include "ThreadsPool/ThreadsPool.h"

#define MAX_RANDOM_INTEGER 25

//#define EXECUTE_TESTS // Comment to disable tests...
#define AUDIT if(1)

int main2(int argc, char **argv) {

    //if ()

    //int i;
    //printf("argc=%d\n", argc);
    //for (i = 0; i < argc; ++i)
    //    printf("argv[%d]:=%s\n", i, argv[i]);

    return 0;
}


int main() {

    DLogProblemInstance *instance = allocateDLogProblemInstance("499", "5", "243255234", MAX_RANDOM_INTEGER);
    DiscreteLogarithm *discreteLogarithm = instance->discreteLogarithmToCompute;

    setSmoothnessBound(instance, "7");

    instance->threadsPoolData = allocateThreadsPoolData(instance);

    startThreadPool(4, &threadRoutineForRelationRetrieval, (void *) instance->threadsPoolData);

    fprintf(stdout, "Start 1° phase...\n");
    startFirstPhase(instance);
    fprintf(stdout, "Start 2° phase...\n");
    startSecondPhase(instance);
    fprintf(stdout, "Start 3° phase...\n");
    startThirdPhase(instance);

    if (isCorrect(discreteLogarithm)) {
        gmp_printf("--> In Z_(%Zd)\n", discreteLogarithm->moduloOfMultiplicativeGroup);
        gmp_printf("--> log_(%Zd) (%Zd) = %Zd\n", discreteLogarithm->base, discreteLogarithm->argument,
                   discreteLogarithm->value);
    } else {
        fprintf(stdout, "Computation error...");
    }

    return 0;
}