
#include <gmp.h>
#include <stdlib.h>
#include <string.h>

#define GMPBufferLength 10

__mpz_struct* getNumberFromGMPBuffer(__mpz_struct **buffer, int index) {
    return *(buffer + index);
}

__mpz_struct **initGMPBuffer() {

    __mpz_struct **output = malloc(GMPBufferLength * sizeof(__mpz_struct*));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        for (int i = 0; i < GMPBufferLength; i++) {

            __mpz_struct *number = malloc(sizeof(__mpz_struct));
            if (number == NULL)
                exit(EXIT_FAILURE);

            mpz_init(number);
            *(output + i) = number;
        }

    return output;
}

void freeGMPBuffer(__mpz_struct **buffer) {

    for (int i = 0; i <= GMPBufferLength; i++) {
        mpz_clear(*(buffer + i));
    }

    free(buffer);
}