#include <stdlib.h>
#include <stdio.h>
#include "UserInput.h"
#include "Math/Number.h"
#include "Error.h"
#include "Math/OrderedFactorList.h"
#include "Math/Factorization.h"
#include "Math/Common.h"


DLogProblemInstanceInput *allocateDLogProblemInstanceInput() {

    DLogProblemInstanceInput *output = malloc(sizeof(DLogProblemInstanceInput));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        return output;
}



DLogProblemInstanceInput *sanitizeRawUserInput(RawUserInput *input, unsigned long maxRandomInteger, unsigned long numbersBufferSize) {

    DLogProblemInstanceInput *output = allocateDLogProblemInstanceInput();

    output->maxRandomInteger = allocateAndSetNumberFromULL(maxRandomInteger);
    output->numbersBuffer = allocateNumbersBuffer(numbersBufferSize);
    output->randomIntegerGenerator = allocateRandomIntegerGenerator(output->maxRandomInteger);

    output->dLogBase = allocateAndSetNumberFromString(input->dLogBase);
    if (output->dLogBase == NULL)
        exitPrintingFatalErrorMessage("getVerifiedDLogProblemInstanceInput", "Invalid 'dLogBase'!");

    output->dLogArgument = allocateAndSetNumberFromString(input->dLogArgument);
    if (output->dLogArgument == NULL)
        exitPrintingFatalErrorMessage("getVerifiedDLogProblemInstanceInput", "Invalid 'dLogArgument'!");

    output->smoothnessBound = allocateAndSetNumberFromString(input->smoothnessBound);
    if (output->smoothnessBound == NULL)
        exitPrintingFatalErrorMessage("getVerifiedDLogProblemInstanceInput", "Invalid 'smoothnessBound'!");

    output->multiplicativeGroup = allocateAndSetNumberFromString(input->multiplicativeGroup);
    if (output->multiplicativeGroup == NULL)
        exitPrintingFatalErrorMessage("getVerifiedDLogProblemInstanceInput", "Invalid 'multiplicativeGroup'!");


    if (isPrime(output->multiplicativeGroup) == false)
        exitPrintingFatalErrorMessage("getVerifiedDLogProblemInstanceInput", "Invalid 'multiplicativeGroup' is NOT prime!");

    if (isGroupGenerator(output->dLogBase, output->multiplicativeGroup, output->numbersBuffer, output->randomIntegerGenerator) == false) {
        gmp_fprintf(stderr, "[WARNING] Specified logarithm's base (%Zd) is NOT a generator in Z_(%Zd)\n", output->dLogBase, output->multiplicativeGroup);
        exitPrintingFatalErrorMessage("getVerifiedDLogProblemInstanceInput", "Invalid 'dLogBase'");
    }

    return output;
}