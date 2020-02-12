//
// Created by andrea on 20/11/19.
//

#include <gmp.h>
#include <stdbool.h>
#include "Common.h"
#include "Number.h"
#include "RandomNumber.h"
#include "OrderedFactorList.h"
#include "Factorization.h"
#include "../Error.h"


#define DEFINITELY_NOT_PRIME 0

bool isPrime(__mpz_struct *input) {

    switch (mpz_probab_prime_p(input, 1)) {
        case DEFINITELY_NOT_PRIME:
            return false;
        default:
            return true;
    }
}

bool isGroupGenerator(__mpz_struct *input, __mpz_struct *multiplicativeGroup, NumbersBuffer *numbersBuffer, RandomIntegerGenerator *randomIntegerGenerator, bool suppressOutput) {

    bool output = true;

    __mpz_struct **buffer = retrieveNumbersFromBuffer(numbersBuffer, 4);

    __mpz_struct *possibleGenerator = buffer[0];
    __mpz_struct *result = buffer[1];
    __mpz_struct *exponent = buffer[2];
    __mpz_struct *multiplicativeGroupMinusOne = buffer[3];

    mpz_set(possibleGenerator, input);
    mpz_sub_ui(multiplicativeGroupMinusOne, multiplicativeGroup, 1);

    OrderedFactorList *list = factorizeCheckingBSmoothness(multiplicativeGroupMinusOne, NULL, numbersBuffer, randomIntegerGenerator);
    if (list == NULL)
        exitPrintingFatalErrorMessage("isGroupGenerator", "Error while searching prime divisors of 'multiplicativeGroupMinusOne'.");
    else {
        OrderedFactorListNode *currentNode = list->head;

        while (currentNode != NULL) {
            mpz_sub_ui(exponent, multiplicativeGroup, 1);
            mpz_div(exponent, exponent, currentNode->factor->base);
            mpz_powm(result, possibleGenerator, exponent, multiplicativeGroup);

            if (mpz_cmp_ui(result, 1) == 0) {

                if (suppressOutput == false)
                    gmp_printf("[WARNING] In Z_(%Zd): %Zd^(%Zd-1/%Zd) = %Zd\n", multiplicativeGroup, possibleGenerator, multiplicativeGroup, currentNode->factor->base, result);

                output = false;
                break;
            } else
                currentNode = currentNode->next_node;
        }
    }
    releaseNumbers(numbersBuffer, 4);
    freeOrderedFactorList(list);
    return output;
}


bool isInvertible(NumbersBuffer *numbersBuffer, mpz_t input, mpz_t modulo) {

    __mpz_struct *gcd = retrieveNumberFromBuffer(numbersBuffer);

    mpz_gcd(gcd, input, modulo);
    bool output = (mpz_cmp_ui(gcd, 1) == 0);

    releaseNumber(numbersBuffer);

    return output;
}