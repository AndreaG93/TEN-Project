#include "IndexCalculusAlgorithm/DLogProblemInstance.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/ThirdStep.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/SecondStep.h"
#include "IndexCalculusAlgorithm/AlgorithmSteps/FirstStep.h"
#include "IndexCalculusAlgorithm/UserInput.h"
#include "Test/Tests.h"

#define MAX_RANDOM_INTEGER 25
#define NUMBER_BUFFER_LENGTH 25
#define POOL_SIZE 1

RawUserInput rawUserInput;

int main(int argc, char **argv) {

    /*
    Matrix *matrix = allocateMatrix(3, 5);

    setNumberMatrixCell(matrix, 0,0, allocateAndSetNumberFromString("0"));
    setNumberMatrixCell(matrix, 0,1, allocateAndSetNumberFromString("2"));
    setNumberMatrixCell(matrix, 0,2, allocateAndSetNumberFromString("1"));
    setNumberMatrixCell(matrix, 0,3, allocateAndSetNumberFromString("0"));
    setNumberMatrixCell(matrix, 0,4, allocateAndSetNumberFromString("-2"));

    setNumberMatrixCell(matrix, 1,0, allocateAndSetNumberFromString("0"));
    setNumberMatrixCell(matrix, 1,1, allocateAndSetNumberFromString("-1"));
    setNumberMatrixCell(matrix, 1,2, allocateAndSetNumberFromString("2"));
    setNumberMatrixCell(matrix, 1,3, allocateAndSetNumberFromString("-1"));
    setNumberMatrixCell(matrix, 1,4, allocateAndSetNumberFromString("-3"));

    setNumberMatrixCell(matrix, 2,0, allocateAndSetNumberFromString("0"));
    setNumberMatrixCell(matrix, 2,1, allocateAndSetNumberFromString("-2"));
    setNumberMatrixCell(matrix, 2,2, allocateAndSetNumberFromString("-1"));
    setNumberMatrixCell(matrix, 2,3, allocateAndSetNumberFromString("1"));
    setNumberMatrixCell(matrix, 2,4, allocateAndSetNumberFromString("-5"));


    printMatrix(matrix);
    performGaussianElimination(matrix, allocateNumbersBuffer(10), allocateAndSetNumberFromString("178"),0);
    printMatrix(matrix);

    return 0;
*/



#ifdef DEBUG
    computeOptimalSmoothnessBound();
    pollardRhoTest();
    factorizationTest();
    factorizationCheckingBSmoothnessTest();
    relationGenerationTest();
    numberAllocationDeAllocation();
    foundGenerator();
    return 0;
#endif

#ifdef RELEASE
    if (argc < 3) {
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

    rawUserInput.dLogBase = "11";
    rawUserInput.dLogArgument = "13";
    rawUserInput.multiplicativeGroup = "179";
    rawUserInput.smoothnessBound = "15";

#endif

    DLogProblemInstanceInput *input = sanitizeRawUserInput(&rawUserInput, MAX_RANDOM_INTEGER, NUMBER_BUFFER_LENGTH);
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
// ================================================
// Prime                    | Smallest Generator
// ================================================
// 97011687217              | 10
// 48112959837082048697     | 3