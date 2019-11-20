#include "DLogProblemInstance.h"
#include "../Math/Number.h"

DLogProblemInstance *
allocateDLogProblemInstance(char *moduloOfMultiplicativeGroup, char *dLogBase, char *dLogArgument) {

    DLogProblemInstance *output = malloc(sizeof(DLogProblemInstance));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {

        output->randomIntegerGenerator = initializeRandomIntegerGenerator();

        output->applicationBuffer = allocateApplicationBuffer();

        output->moduloOfMultiplicativeGroup = allocateAndSetNumberFromString(moduloOfMultiplicativeGroup);
        output->moduloOfMultiplicativeGroupMinusOne = allocateNumber();
        mpz_sub_ui(output->moduloOfMultiplicativeGroupMinusOne, output->moduloOfMultiplicativeGroup, 1);

        output->discreteLogarithmToCompute = allocateDiscreteLogarithm(dLogBase, dLogArgument,
                                                                       output->moduloOfMultiplicativeGroup);

        output->maxRandomInteger = allocateAndSetNumberFromULL(15);

        output->secondPhaseOutput = allocateSecondPhaseOutput(4); //TODO
        output->factorBase = allocateNumbersArray(4); //TODO
    }

    return output;
}