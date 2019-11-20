#include <stdlib.h>
#include "RandomNumber.h"
#include "Number.h"

RandomIntegerGenerator *initializeRandomIntegerGenerator() {

    RandomIntegerGenerator *output = malloc(sizeof(RandomIntegerGenerator));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {

        gmp_randinit_mt(output);
        return output;
    }
}

__mpz_struct* getUniformlyDistributedRandomInteger(RandomIntegerGenerator* generator, __mpz_struct* max) {

    __mpz_struct *output = allocateNumber();
    mpz_urandomm(output, generator, max);

    return output;
}