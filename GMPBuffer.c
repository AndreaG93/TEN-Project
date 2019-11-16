
#include <gmp.h>
#include <stdlib.h>

#define GMPBufferLength 10

mpz_ptr getNumberFromGMPBuffer(mpz_ptr *buffer, int index) {
    return *(buffer + index);
}

mpz_ptr *initGMPBuffer() {

    mpz_ptr *output = malloc(GMPBufferLength);
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        for (int i = 0; i < GMPBufferLength; i++) {

            mpz_ptr number = malloc(sizeof(mpz_ptr));
            if (number == NULL)
                exit(EXIT_FAILURE);

            mpz_init(number);
            *(output + i) = number;
        }

    return output;
}

void freeGMPBuffer(mpz_ptr *buffer) {

    for (int i = 0; i <= GMPBufferLength; i++) {
        mpz_clear(*(buffer + i));
    }

    free(buffer);
}