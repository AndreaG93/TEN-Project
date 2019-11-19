#include "MathCommon_test.h"
#include "Math/NumberBuffer_test.h"
#include "ApplicationBuffer/ApplicationBuffer.h"
#include "Math/OrderedFactorList.h"
#include "Math/Factorization.h"

//#define EXECUTE_TESTS // Comment to disable tests...
#define AUDIT if(1)

int gcd(int a, int b) {
    int remainder;
    while (b != 0) {
        remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

int main() {

/*
    int number = 72, loop = 1, count;
    int x_fixed = 2, x = 1, size = 2, factor;
    int y = 1;
    do {
        printf("----   loop %4i   ----\n", loop);
        count = size;
        do {
            x = (x * x + 1) % number;
            y = (y * y + 1) % number;
            y = (y * y + 1) % number;
            factor = gcd(abs(x - y), number);
            printf("count = %4i  x = %6i  factor = %i\n", size - count + 1, x, factor);
        } while (--count && (factor == 1));
        size *= 2;
        x_fixed = x;
        loop = loop + 1;
    } while (factor == 1);
    printf("factor is %i\n", factor);

    */

    ApplicationBuffer *applicationBuffer = allocateApplicationBuffer();

    __mpz_struct *number = allocateAndSetULLNumber(72);

    OrderedFactorList *orderedFactorList = factorize(applicationBuffer, number);
    OrderedFactorListNode *currentNode = orderedFactorList->head;

    while (currentNode != NULL){
        gmp_printf("%Zd^%Zd ", currentNode->factor->base, currentNode->factor->exponent);
        currentNode = currentNode->next_factor;
    }



#ifdef EXECUTE_TESTS
    numberBuffer_test();
    getInverseMultiplicative_test();
    isInvertible_test();
#endif

    return 0;
}