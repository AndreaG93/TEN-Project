
#include "DiscreteLogarithm.h"
#include "../Math/Number.h"

DiscreteLogarithm *allocateDiscreteLogarithm(__mpz_struct *base, __mpz_struct *argument, __mpz_struct *moduloOfMultiplicativeGroup) {

    DiscreteLogarithm *output = malloc(sizeof(DiscreteLogarithm));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->argument = argument;
        output->base = base;
        output->value = allocateNumber();
        output->multiplicativeGroupMinusOne = allocateNumber();
        output->multiplicativeGroup = moduloOfMultiplicativeGroup;
        mpz_sub_ui(output->multiplicativeGroupMinusOne, output->multiplicativeGroup, 1);
        output->multiplicativeGroupDouble = allocateNumber();
        mpz_mul_ui(output->multiplicativeGroupDouble, output->multiplicativeGroup, 2);
    }

    return output;
}

void freeDiscreteLogarithm(DiscreteLogarithm *input) {
    freeNumber(input->argument);
    freeNumber(input->base);
    freeNumber(input->multiplicativeGroup);
    freeNumber(input->multiplicativeGroupMinusOne);
    freeNumber(input->value);
    free(input->multiplicativeGroupDouble);
}

bool isCorrect(DiscreteLogarithm *dLog) {

    bool output;
    __mpz_struct *computedDLogArgument = allocateNumber();

    mpz_powm(computedDLogArgument, dLog->base, dLog->value, dLog->multiplicativeGroup);

    if (mpz_cmp(computedDLogArgument, dLog->argument) == 0)
        output = true;
    else
        output = false;

    mpz_clear(computedDLogArgument);
    free(computedDLogArgument);

    return output;
}