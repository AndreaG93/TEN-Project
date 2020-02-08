#include "IndexCalculusAlgorithm/DLogProblemInstance.h"
#include "IndexCalculusAlgorithm/RelationsRetrieval.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/ThirdStep.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/SecondStep.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/FirstStep.h"
#include "ThreadsPool/ThreadsPool.h"
#include "Math/OrderedFactorList.h"
#include "Math/Factorization.h"
#include "Math/Number.h"
#include "UserInput.h"

#define MAX_RANDOM_INTEGER 100000000
#define NUMBER_BUFFER_LENGTH 30
#define DEBUG

RawUserInput rawUserInput;

int main(int argc, char **argv) {

    if (argc == 4) { //TODO set to != 4 --> MOREOVER SEE BELOW
        fprintf(stderr, "USAGE: %s [dLogBase] [dLogArgument] [multiplicativeGroup] [smoothnessBound]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    rawUserInput.dLogBase = "11"; //argv[1];
    rawUserInput.dLogArgument = "13";// argv[2];
    rawUserInput.multiplicativeGroup = "179"; //argv[3];
    rawUserInput.smoothnessBound = "17"; //argv[4];

    DLogProblemInstanceInput *input = sanitizeRawUserInput(&rawUserInput, MAX_RANDOM_INTEGER, NUMBER_BUFFER_LENGTH);
    DLogProblemInstance *dLogProblemInstance = allocateDLogProblemInstance(input);
    free(input);

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
        gmp_printf("--> In Z_(%Zd)\n", output->multiplicativeGroup);
        gmp_printf("--> log_(%Zd) (%Zd) = %Zd\n", output->base, output->argument, output->value);
    } else {
        fprintf(stdout, "== FAILURE ==\n");
        gmp_printf("--> In Z_(%Zd)\n", output->multiplicativeGroup);
        gmp_printf("--> log_(%Zd) (%Zd) != %Zd\n", output->base, output->argument, output->value);
    }

    return 0;
}