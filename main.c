#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include "DataStructures/UnorderedList.h"
#include "routine_test.h"
#include "GMPBuffer.h"

#define AUDIT if(1)


void PollardSRhoFunction(mpz_t x, mpz_t modulo) {
    mpz_mul(x, x, x);
    mpz_add_ui(x, x, 2);
    mpz_mod(x, x, modulo);
}










void printFactor(void **data) {
    fprintf(stderr, "Factor: %s\n", mpz_get_str(NULL, 10, *data));
}



UnorderedList *PollardSRhoAlgorithm(mpz_t input) {

    UnorderedList *output = allocUnorderedList();

    mpz_t x;
    mpz_t y;
    mpz_t XMinusY;
    mpz_t factor;

    mpz_init(XMinusY);
    mpz_init(factor);

    while (1) {

        mpz_init_set_ui(x, 1);
        mpz_init_set_ui(y, 1);

        while (1) {

            PollardSRhoFunction(x, input);
            PollardSRhoFunction(y, input);
            PollardSRhoFunction(y, input);

            mpz_sub(XMinusY, x, y);
            mpz_gcd(factor, XMinusY, input);

            AUDIT printf("X       : %s\n", mpz_get_str(NULL, 10, x));
            AUDIT printf("Y       : %s\n", mpz_get_str(NULL, 10, y));
            AUDIT printf("GCD(X,Y): %s\n", mpz_get_str(NULL, 10, factor));

            if (mpz_cmp_si(factor, 1) != 0) {

                mpz_t newFactor;
                mpz_init_set(newFactor, factor);

                insert(output, newFactor);
                AUDIT printf("Found Factor: %s\n", mpz_get_str(NULL, 10, newFactor));
                break;
            }
            else if (mpz_cmp(factor, input) == 0)
                break;
        }

        mpz_div(input, input, factor);
        if (mpz_cmp_si(input, 1) == 0) {
            break;
        }
    }

    mpz_clear(XMinusY);
    mpz_clear(y);
    mpz_clear(x);

    return output;
}

int main() {



    getMultiplicativeInverseWithExtendedEuclideanAlgorithm_test();
//computeFactorBase_test();

    /*
    mpz_t input;


    if (mpz_init_set_str(input, "8051", 10) != 0)
        return EXIT_FAILURE;

    UnorderedList *factorList = PollardSRhoAlgorithm(input);

    performAction(factorList, &printFactor);
*/
    return 0;
}