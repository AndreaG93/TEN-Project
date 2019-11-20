#include "DLogProblemInstance.h"
#include "../Math/Number.h"

DLogProblemInstance *
allocateDLogProblemInstance(char *moduloOfMultiplicativeGroup, char *dLogBase, char *dLogArgument) {

    DLogProblemInstance *output = malloc(sizeof(DLogProblemInstance));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {

        output->randomIntegerGenerator = initializeRandomNumberGenerator();

        output->applicationBuffer = allocateApplicationBuffer();

        output->moduloOfMultiplicativeGroup = allocateAndSetNumberFromString(moduloOfMultiplicativeGroup);

        output->discreteLogarithmToCompute = allocateDiscreteLogarithm(dLogBase, dLogArgument,
                                                                       output->moduloOfMultiplicativeGroup);

    }

    return output;
}