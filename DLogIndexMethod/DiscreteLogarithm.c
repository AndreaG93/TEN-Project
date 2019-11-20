
#include "DiscreteLogarithm.h"
#include "../Math/Number.h"
#include "../ApplicationBuffer/ApplicationBuffer.h"

DiscreteLogarithm *allocateDiscreteLogarithm(char *base, char *argument, __mpz_struct* moduloOfMultiplicativeGroup) {

    DiscreteLogarithm *output = malloc(sizeof(DiscreteLogarithm));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->argument = allocateAndSetNumberFromString(argument);
        output->base = allocateAndSetNumberFromString(base);
        output->moduloOfMultiplicativeGroup = moduloOfMultiplicativeGroup;
        output->value = allocateNumber();
    }
    return output;
}

bool isCorrect(DiscreteLogarithm *dLog, ApplicationBuffer *appBuffer) {

    __mpz_struct *computedDLogArgument = getAuxiliaryNumber(appBuffer, 0);

    mpz_powm(computedDLogArgument, dLog->base, dLog->value, dLog->moduloOfMultiplicativeGroup);

    if (mpz_cmp(computedDLogArgument, dLog->argument) == 0)
        return true;
    else
        return false;
}