#include <stdlib.h>
#include <stdio.h>
#include "UserInput.h"
#include "Math/Number.h"
#include "Error.h"
#include "Math/OrderedFactorList.h"
#include "Math/Factorization.h"

#define DEFINITELY_NOT_PRIME 0

DLogProblemInstanceInput *allocateDLogProblemInstanceInput() {

    DLogProblemInstanceInput *output = malloc(sizeof(DLogProblemInstanceInput));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        return output;
}

bool isPrime(__mpz_struct *input) {

    switch (mpz_probab_prime_p(input, 1)) {
        case DEFINITELY_NOT_PRIME:
            return false;
        default:
            return true;
    }
}

bool isGroupGenerator(__mpz_struct *input, __mpz_struct *multiplicativeGroup, NumbersBuffer *numbersBuffer,
                      RandomIntegerGenerator *randomIntegerGenerator) {

    bool output = true;

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 4);

    __mpz_struct *possibleGenerator = buffer[0];
    __mpz_struct *result = buffer[1];
    __mpz_struct *exponent = buffer[2];
    __mpz_struct *multiplicativeGroupMinusOne = buffer[3];

    mpz_set(possibleGenerator, input);
    mpz_sub_ui(multiplicativeGroupMinusOne, multiplicativeGroup, 1);

    OrderedFactorList *list = factorize(multiplicativeGroupMinusOne, numbersBuffer, randomIntegerGenerator);
    if (list == NULL)
        exitPrintingFatalErrorMessage("isGroupGenerator", "Invalid 'dLogBase'!");

    OrderedFactorListNode *currentNode = list->head;

    while (currentNode != NULL) {
        mpz_sub_ui(exponent, multiplicativeGroup, 1);
        mpz_div(exponent, exponent, currentNode->factor->base);
        mpz_powm(result, possibleGenerator, exponent, multiplicativeGroup);

        if (mpz_cmp_ui(result, 1) == 0) {
            output = false;
            break;
        } else
            currentNode = currentNode->next_node;
    }

    releaseNumbers(numbersBuffer, 4);
    deallocateOrderedFactorList(list);
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