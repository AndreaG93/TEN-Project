#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "RandomNumber.h"
#include "Number.h"

RandomIntegerGenerator *allocateRandomIntegerGenerator(__mpz_struct *maxRandomInteger) {

    RandomIntegerGenerator *output = malloc(sizeof(RandomIntegerGenerator));
    if (output == NULL)
        exit(EXIT_FAILURE);

    char myRandomData[50];
    getrandom()

    unsigned long long seed = strtoul(myRandomData, NULL, 10);

    gmp_randinit_mt(&output->state);
    gmp_randseed_ui(&output->state, seed);
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
