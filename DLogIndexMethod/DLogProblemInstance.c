#include "DLogProblemInstance.h"
#include "../Math/Number.h"

DLogProblemInstance *
allocateDLogProblemInstance(char *moduloOfMultiplicativeGroup, char *dLogBase, char *dLogArgument) {

    DLogProblemInstance *output = malloc(sizeof(DLogProblemInstance));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {

        output->applicationBuffer = allocateApplicationBuffer();

        output->moduloOfMultiplicativeGroup = allocateAndSetNumberFromString(moduloOfMultiplicativeGroup);
        output->moduloOfMultiplicativeGroupMinusOne = allocateNumber();
        mpz_sub_ui(output->moduloOfMultiplicativeGroupMinusOne, output->moduloOfMultiplicativeGroup, 1);

        output->discreteLogarithmToCompute = allocateDiscreteLogarithm(dLogBase, dLogArgument,
                                                                       output->moduloOfMultiplicativeGroup);
    }

    return output;
}

void setSmoothnessBound(DLogProblemInstance *instance, char *smoothnessBoundAsString) {
    instance->smoothnessBound = allocateAndSetNumberFromString(smoothnessBoundAsString);
}

void initializeRandIntegerGenerator(DLogProblemInstance *instance, unsigned long long maxRandomInteger) {

    instance->randomIntegerGenerator = initializeRandomIntegerGenerator();
    instance->maxRandomInteger = allocateAndSetNumberFromULL(maxRandomInteger);
}