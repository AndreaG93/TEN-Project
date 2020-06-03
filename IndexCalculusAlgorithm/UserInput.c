#include <stdlib.h>
#include <stdio.h>
#include "UserInput.h"
#include "../Math/Number.h"
#include "../Error.h"
#include "../Math/OrderedFactorList.h"
#include "../Math/Common.h"
#include <math.h>


DLogProblemInstanceInput *allocateDLogProblemInstanceInput() {

    DLogProblemInstanceInput *output = malloc(sizeof(DLogProblemInstanceInput));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        return output;
}

__mpz_struct* computeOptimalSmoothnessBound(unsigned long primeNumber) {

    double smoothnessBound = exp(sqrt(((log( (double) primeNumber)*log(log( (double) primeNumber)))/3)));

    return allocateAndSetNumberFromULL( smoothnessBound * 2);
}

DLogProblemInstanceInput *sanitizeRawUserInput(RawUserInput *input, unsigned long numbersBufferSize) {

    DLogProblemInstanceInput *output = allocateDLogProblemInstanceInput();

    output->numbersBuffer = allocateNumbersBuffer(numbersBufferSize);

    output->dLogBase = allocateAndSetNumberFromString(input->dLogBase);
    if (output->dLogBase == NULL)
        exitPrintingFatalErrorMessage("getVerifiedDLogProblemInstanceInput", "Invalid 'dLogBase'!");

    output->dLogArgument = allocateAndSetNumberFromString(input->dLogArgument);
    if (output->dLogArgument == NULL)
        exitPrintingFatalErrorMessage("getVerifiedDLogProblemInstanceInput", "Invalid 'dLogArgument'!");

    output->multiplicativeGroup = allocateAndSetNumberFromString(input->multiplicativeGroup);
    if (output->multiplicativeGroup == NULL)
        exitPrintingFatalErrorMessage("getVerifiedDLogProblemInstanceInput", "Invalid 'multiplicativeGroup'!");

    if (input->smoothnessBound == NULL) {

        output->smoothnessBound = computeOptimalSmoothnessBound(mpz_get_ui(output->multiplicativeGroup));
        gmp_fprintf(stderr, "[INFO] Using optimal theoretical value of smoothness bound: B = %Zd\n", output->smoothnessBound);

    } else {

        output->smoothnessBound = allocateAndSetNumberFromString(input->smoothnessBound);
        if (output->smoothnessBound == NULL)
            exitPrintingFatalErrorMessage("getVerifiedDLogProblemInstanceInput", "Invalid 'smoothnessBound'!");
    }

    if (isPrime(output->multiplicativeGroup) == false)
        exitPrintingFatalErrorMessage("getVerifiedDLogProblemInstanceInput", "Invalid 'multiplicativeGroup' is NOT prime!");

    output->randomIntegerGenerator = allocateRandomIntegerGenerator(output->multiplicativeGroup);

    mpz_mod(output->dLogArgument, output->dLogArgument, output->multiplicativeGroup);

    return output;
}