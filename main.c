#include "IndexCalculusAlgorithm/DLogProblemInstance.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/ThirdStep.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/SecondStep.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/FirstStep.h"
#include "IndexCalculusAlgorithm/UserInput.h"

#define NUMBER_BUFFER_LENGTH 30
#define POOL_SIZE 4
#define RELEASE

RawUserInput rawUserInput;

int main(int argc, char **argv) {

#ifdef RELEASE

    if (argc < 4) {
        fprintf(stderr, "USAGE: %s [dLogBase] [dLogArgument] [multiplicativeGroup] [ (OPTIONAL) smoothnessBound]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    rawUserInput.dLogBase = argv[1];
    rawUserInput.dLogArgument = argv[2];
    rawUserInput.multiplicativeGroup = argv[3];

    if (argc == 5)
        rawUserInput.smoothnessBound = argv[4];
    else
        rawUserInput.smoothnessBound = NULL;

#endif
#ifndef RELEASE

    // Used for tests...

    rawUserInput.dLogBase = "7";
    rawUserInput.dLogArgument = "13";
    rawUserInput.multiplicativeGroup = "481129654654645654645656013";
    rawUserInput.smoothnessBound = NULL;

#endif

    DLogProblemInstanceInput *input = sanitizeRawUserInput(&rawUserInput, NUMBER_BUFFER_LENGTH);
    DLogProblemInstance *dLogProblemInstance = allocateDLogProblemInstance(input, POOL_SIZE);
    free(input);

    gmp_fprintf(stderr, "[INFO] Start computing log_(%Zd) (%Zd) in Z*_(%Zd)\n", dLogProblemInstance->discreteLogarithm->base, dLogProblemInstance->discreteLogarithm->argument, dLogProblemInstance->discreteLogarithm->multiplicativeGroup);
    gmp_fprintf(stderr, "[INFO] Smoothness Bound set to %Zd\n", dLogProblemInstance->smoothnessBound);

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

    freeDLogProblemInstance(dLogProblemInstance);

    return 0;
}

// SOME PRIME USED FOR TESTS
// =================================================================================
// Prime                             | LogBase     | Argument       | Time
// =================================================================================
// 97011687217                       | 10          | 157            | 0m0.064s
// 48112959837082048697              | 3           | 100            | 0m8.033s
// 4811296546546456546456559         | 7           | 22             | 2m33.401s