#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

int main() {
    mpz_t input;

    if (mpz_init_set_str(input, "32gasda452", 10) != 0)
        return EXIT_FAILURE;

    printf(" %s\n", mpz_get_str(NULL, 10, input));



    printf("Hello, World!\n");
    return 0;
}