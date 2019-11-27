
#include "DiscreteLogarithm.h"
#include "../Math/Number.h"

DiscreteLogarithm *allocateDiscreteLogarithm(char *base, char *argument, __mpz_struct *moduloOfMultiplicativeGroup) {

    DiscreteLogarithm *output = malloc(sizeof(DiscreteLogarithm));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->argument = allocateAndSetNumberFromString(argument);
        output->base = allocateAndSetNumberFromString(base);
        output->moduloOfMultiplicativeGroup = moduloOfMultiplicativeGroup;
        output->value = allocateNumber();
    }

    mpz_mod(output->argument, output->argument, output->moduloOfMultiplicativeGroup);
    mpz_mod(output->base, output->base, output->moduloOfMultiplicativeGroup);

    return output;
}

bool isCorrect(DiscreteLogarithm *dLog) {

    bool output;
    __mpz_struct *computedDLogArgument = allocateNumber();

    mpz_powm(computedDLogArgument, dLog->base, dLog->value, dLog->moduloOfMultiplicativeGroup);

    if (mpz_cmp(computedDLogArgument, dLog->argument) == 0)
        output = true;
    else
        output = false;

    mpz_clear(computedDLogArgument);
    free(computedDLogArgument);

    return output;
}