#include <stdlib.h>
#include "RandomNumber.h"
#include "Number.h"

RandomIntegerGenerator *allocateRandomIntegerGenerator(__mpz_struct *maxRandomInteger) {

    RandomIntegerGenerator *output = malloc(sizeof(RandomIntegerGenerator));
    if (output == NULL)
        exit(EXIT_FAILURE);

    gmp_randinit_mt(&output->state);
    output->maxRandomInteger = maxRandomInteger;

    return output;
}

void selectUniformlyDistributedRandomInteger(RandomIntegerGenerator *input, __mpz_struct *output) {
    mpz_urandomm(output, &input->state, input->maxRandomInteger);
}

void deallocateRandomIntegerGenerator(RandomIntegerGenerator *input) {

    gmp_randclear(&input->state);
    free(input);
}
