#include "IndexCalculusAlgorithm/DLogProblemInstance.h"
#include "IndexCalculusAlgorithm/RelationsRetrieval.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/ThirdStep.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/SecondStep.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/FirstStep.h"
#include "ThreadsPool/ThreadsPool.h"

#define MAX_RANDOM_INTEGER 100000000
#define NUMBER_BUFFER_LENGTH 30
#define DEBUG

int main() {

    DLogProblemInstance *dLogProblemInstance = allocateAndInitializeDLogProblemInstance("179", "11", "13", "7", MAX_RANDOM_INTEGER, NUMBER_BUFFER_LENGTH);

    startThreadPool(4, &threadRoutineForRelationRetrieval, (void *) dLogProblemInstance->threadsPoolData);

    fprintf(stderr, "Start 1° algorithm step...\n");
    startFirstStep(dLogProblemInstance);
    fprintf(stderr, "Start 2° algorithm step...\n");
    startSecondStep(dLogProblemInstance);
    fprintf(stderr, "Start 3° algorithm step...\n");
    startThirdStep(dLogProblemInstance);

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