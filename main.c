#include "DLogIndexMethod/DLogProblemInstance.h"
#include "DLogIndexMethod/ThirdPhase.h"
#include "DLogIndexMethod/FirstPhase.h"
#include "DLogIndexMethod/SecondPhase.h"
#include "DLogIndexMethod/RelationsRetrieval.h"
#include "ThreadsPool/ThreadsPool.h"
#include "Math/Number.h"
#include "Math/Factorization.h"

#define MAX_RANDOM_INTEGER 100000000
#define NUMBER_BUFFER_LENGTH 30

#define DEBUG

//#define EXECUTE_TESTS // Comment to disable tests...
#define AUDIT if(1)

int main() {

    DLogProblemInstance *dLogProblemInstance = allocateDLogProblemInstance();

    dLogProblemInstance->numbersBuffer = allocateNumbersBuffer(NUMBER_BUFFER_LENGTH);
    dLogProblemInstance->randomIntegerGenerator = allocateRandomIntegerGenerator(allocateAndSetNumberFromULL(MAX_RANDOM_INTEGER));
    dLogProblemInstance->smoothnessBound = allocateAndSetNumberFromString("7");
    dLogProblemInstance->threadsPoolData = allocateThreadsPoolData(dLogProblemInstance);
    dLogProblemInstance->moduloOfMultiplicativeGroup = allocateAndSetNumberFromString("179");
    dLogProblemInstance->moduloOfMultiplicativeGroupMinusOne = allocateNumber();
    dLogProblemInstance->discreteLogarithm = allocateDiscreteLogarithm("11", "13", dLogProblemInstance->moduloOfMultiplicativeGroup);

    startThreadPool(4, &threadRoutineForRelationRetrieval, (void *) dLogProblemInstance->threadsPoolData);

    fprintf(stderr, "Start 1° phase...\n");
    startFirstPhase(dLogProblemInstance);
    fprintf(stderr, "Start 2° phase...\n");
    startSecondPhase(dLogProblemInstance);
    fprintf(stderr, "Start 3° phase...\n");
    startThirdPhase(dLogProblemInstance);

    DiscreteLogarithm *output = dLogProblemInstance->discreteLogarithm;

    if (isCorrect(output)) {
        fprintf(stdout, "== SUCCESS ==\n");
        gmp_printf("--> In Z_(%Zd)\n", output->moduloOfMultiplicativeGroup);
        gmp_printf("--> log_(%Zd) (%Zd) = %Zd\n", output->base, output->argument,
                   output->value);
    } else {
        fprintf(stdout, "== FAILURE ==\n");
        gmp_printf("--> In Z_(%Zd)\n", output->moduloOfMultiplicativeGroup);
        gmp_printf("--> log_(%Zd) (%Zd) != %Zd\n", output->base, output->argument,
                   output->value);
    }

    return 0;
}