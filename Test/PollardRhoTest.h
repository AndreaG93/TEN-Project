#pragma oncemark

#include <gmp.h>
#include "../Math/Number.h"
#include "../Buffers/NumbersBuffer.h"
#include "../Math/RandomNumber.h"
#include "../Math/Factorization.h"
#include "../IndexCalculusAlgorithm/RelationsRetrieval.h"
#include "../Math/Matrix.h"

void pollardRhoTest() {

    NumbersBuffer* numbersBuffer = allocateNumbersBuffer(30);
    RandomIntegerGenerator *randomIntegerGenerator = allocateRandomIntegerGenerator(allocateAndSetNumberFromULL(1000));
    __mpz_struct* numberToFactorize = allocateAndSetNumberFromString("39523532652362542342342342334623452344");

    __mpz_struct *output = getFactorUsingPollardRho(numberToFactorize, numbersBuffer, randomIntegerGenerator);

    if (output == NULL)
        fprintf(stderr, "Pollard Rho failed...");
    else
        gmp_fprintf(stderr, "Found Factor %Zd\n", output);
}

void FactorizationTest() {

    NumbersBuffer* numbersBuffer = allocateNumbersBuffer(30);
    __mpz_struct* numberToFactorize = allocateAndSetNumberFromString("39523564564");
    __mpz_struct* maxRandomInteger = allocateAndSetNumberFromULL(1000);

    RandomIntegerGenerator *randomIntegerGenerator = allocateRandomIntegerGenerator(maxRandomInteger);


    OrderedFactorList *output = factorizeCheckingBSmoothness(numberToFactorize, NULL, numbersBuffer, randomIntegerGenerator);

    if (output == NULL)
        fprintf(stderr, "Factorization failed...");
    else
        printOrderedFactorList(output);

    freeOrderedFactorList(output);
    freeNumber(numberToFactorize);
    freeNumber(maxRandomInteger);
    freeNumbersBuffer(numbersBuffer);
    freeRandomIntegerGenerator(randomIntegerGenerator);
}

void FactorizationCheckingBSmoothnessTest() {

    NumbersBuffer* numbersBuffer = allocateNumbersBuffer(30);
    RandomIntegerGenerator *randomIntegerGenerator = allocateRandomIntegerGenerator(allocateAndSetNumberFromULL(1000));
    __mpz_struct* numberToFactorize = allocateAndSetNumberFromString("395235645645654643123123123");      // 27 digits
    __mpz_struct* smoothnessBound = allocateAndSetNumberFromString("395235645645654643123123123896");

    OrderedFactorList *output = factorizeCheckingBSmoothness(numberToFactorize, smoothnessBound, numbersBuffer, randomIntegerGenerator);

    if (output == NULL)
        fprintf(stderr, "Factorization failed...");
    else
        printOrderedFactorList(output);
}

void RelationGenerationTest() {

    RawUserInput rawUserInput;

    rawUserInput.dLogBase = "2";
    rawUserInput.dLogArgument = "500";
    rawUserInput.multiplicativeGroup = "104723";
    rawUserInput.smoothnessBound = "300";

    DLogProblemInstanceInput *input = sanitizeRawUserInput(&rawUserInput, 1000, 40);
    DLogProblemInstance *instance = allocateDLogProblemInstance(input);

    startFirstStep(instance);

    __mpz_struct **relation = getLogarithmRelation(instance, instance->numbersBuffer, instance->randomIntegerGenerator, NULL);

    freeNumbersArray(relation, instance->factorBase->length);
    freeDLogProblemInstance(instance);
    free(input);
}

void NumberAllocationDeAllocation() {

    __mpz_struct* number = allocateAndSetNumberFromString("1000");
    freeNumber(number);

    __mpz_struct** numberArray = allocateNumbersArray(10, true);
    freeNumbersArray(numberArray, 10);

    __mpz_struct* numberAllocatedButNotInitialized = allocateNumber();
    freeNumber(numberAllocatedButNotInitialized);

    Matrix *matrix = allocateMatrix(10, 10);
    freeMatrix(matrix);
}