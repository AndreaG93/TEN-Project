#include "DLogProblemInstance.h"
#include "../Math/Number.h"

DLogProblemInstance *
allocateDLogProblemInstance(char *moduloOfMultiplicativeGroup, char *dLogBase, char *dLogArgument, unsigned long long maxRandomInteger) {

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

        output->maxRandomInteger = allocateAndSetNumberFromULL(maxRandomInteger);
    }

    return output;
}

void setSmoothnessBound(DLogProblemInstance *instance, char *smoothnessBoundAsString) {
    instance->smoothnessBound = allocateAndSetNumberFromString(smoothnessBoundAsString);
}