#include "DLogIndexMethod/DLogProblemInstance.h"
#include "DLogIndexMethod/ThirdPhase.h"
#include "DLogIndexMethod/FirstPhase.h"
#include "DLogIndexMethod/SecondPhase.h"
#include "DLogIndexMethod/RelationsRetrieval.h"
#include "ThreadsPool/ThreadsPool.h"
#include "Math/Number.h"
#include "Math/Factorization.h"

#define MAX_RANDOM_INTEGER 35

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

    DLogProblemInstance *instance = allocateDLogProblemInstance("103", "5", "27", MAX_RANDOM_INTEGER);
    DiscreteLogarithm *discreteLogarithm = instance->discreteLogarithmToCompute;

    __mpz_struct *input = allocateAndSetNumberFromString("1607");
    __mpz_struct *modulo = allocateAndSetNumberFromString("100998");

    factorize(instance->applicationBuffer, input, modulo );
    return 0;









    setSmoothnessBound(instance, "9");

    instance->threadsPoolData = allocateThreadsPoolData(instance);

    startThreadPool(4, &threadRoutineForRelationRetrieval, (void *) instance->threadsPoolData);

    fprintf(stdout, "Start 1° phase...\n");
    startFirstPhase(instance);
    fprintf(stdout, "Start 2° phase...\n");
    startSecondPhase(instance);
    fprintf(stdout, "Start 3° phase...\n");
    startThirdPhase(instance);

    if (isCorrect(discreteLogarithm)) {
        fprintf(stdout, "== SUCCESS ==\n");
        gmp_printf("--> In Z_(%Zd)\n", discreteLogarithm->moduloOfMultiplicativeGroup);
        gmp_printf("--> log_(%Zd) (%Zd) = %Zd\n", discreteLogarithm->base, discreteLogarithm->argument,
                   discreteLogarithm->value);
    } else {
        fprintf(stdout, "== FAILURE ==\n");
        gmp_printf("--> In Z_(%Zd)\n", discreteLogarithm->moduloOfMultiplicativeGroup);
        gmp_printf("--> log_(%Zd) (%Zd) != %Zd\n", discreteLogarithm->base, discreteLogarithm->argument,
                   discreteLogarithm->value);
    }

    return 0;
}