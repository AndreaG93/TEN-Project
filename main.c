#include "IndexCalculusAlgorithm/DLogProblemInstance.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/ThirdStep.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/SecondStep.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/FirstStep.h"
#include "IndexCalculusAlgorithm/UserInput.h"
#include "Test/PollardRhoTest.h"

#define MAX_RANDOM_INTEGER 100
#define NUMBER_BUFFER_LENGTH 25

RawUserInput rawUserInput;

int main(int argc, char **argv) {

#ifdef DEBUG
    //pollardRhoTest();
    //FactorizationTest();
    //FactorizationCheckingBSmoothnessTest();
    //RelationGenerationTest();
    //NumberAllocationDeAllocation();
    //return 0;
#endif

    if (argc == 4) { //TODO set to != 4 --> MOREOVER SEE BELOW
        fprintf(stderr, "USAGE: %s [dLogBase] [dLogArgument] [multiplicativeGroup] [smoothnessBound]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    rawUserInput.dLogBase = "2";
    rawUserInput.dLogArgument = "500";
    rawUserInput.multiplicativeGroup = "104723";
    rawUserInput.smoothnessBound = "300";

    DLogProblemInstanceInput *input = sanitizeRawUserInput(&rawUserInput, MAX_RANDOM_INTEGER, NUMBER_BUFFER_LENGTH);
    DLogProblemInstance *dLogProblemInstance = allocateDLogProblemInstance(input);
    free(input);

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