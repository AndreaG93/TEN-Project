#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "RandomNumber.h"
#include "Number.h"

RandomIntegerGenerator *allocateRandomIntegerGenerator(__mpz_struct *maxRandomInteger) {

    RandomIntegerGenerator *output = malloc(sizeof(RandomIntegerGenerator));
    if (output == NULL)
        exit(EXIT_FAILURE);

    unsigned long int seed;

    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, &seed, sizeof(unsigned long int));
    close(fd);

    gmp_randinit_mt(&output->state);
    gmp_randseed_ui(&output->state, seed);
    output->maxRandomInteger = maxRandomInteger;

    return output;
}

__mpz_struct* selectUniformlyDistributedRandomInteger(RandomIntegerGenerator *input) {

    __mpz_struct* output = allocateNumber();
    mpz_urandomm(output, &input->state, input->maxRandomInteger);

    return output;
}

void freeRandomIntegerGenerator(RandomIntegerGenerator *input) {

    gmp_randclear(&input->state);
    free(input);
}
