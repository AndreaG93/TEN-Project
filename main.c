#include "MathCommon_test.h"
#include "Math/NumberBuffer_test.h"
#include "ApplicationBuffer/ApplicationBuffer.h"
#include "Math/OrderedFactorList.h"
#include "Math/Factorization.h"

//#define EXECUTE_TESTS // Comment to disable tests...
#define AUDIT if(1)


int main() {





    ApplicationBuffer *applicationBuffer = allocateApplicationBuffer();

    __mpz_struct *number = allocateAndSetULLNumber(7242342342352);

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