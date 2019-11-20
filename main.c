#include "ApplicationBuffer/ApplicationBuffer.h"
#include "Math/OrderedFactorList.h"
#include "Math/Factorization.h"
#include "Math/Number.h"
#include "DLogIndexMethod/DLogProblemInstance.h"

//#define EXECUTE_TESTS // Comment to disable tests...
#define AUDIT if(1)


int main() {

    DLogProblemInstance* instance = allocateDLogProblemInstance("179", "2", "13");
    instance->primitiveRoot = allocateAndSetNumberFromString("2");

    __mpz_struct *max = allocateAndSetNumberFromString("253");
    __mpz_struct *randomNumber1 = getUniformlyDistributedRandomInteger(instance->randomIntegerGenerator, max);
    __mpz_struct *randomNumber2 = getUniformlyDistributedRandomInteger(instance->randomIntegerGenerator, max);

    gmp_printf("%Zd", randomNumber1);
    gmp_printf("%Zd", randomNumber2);

/*
    ApplicationBuffer *applicationBuffer = allocateApplicationBuffer();

  __mpz_struct *number = allocateAndSetNumberFromULL(7242342342352);

    OrderedFactorList *orderedFactorList = factorize(applicationBuffer, number);
    OrderedFactorListNode *currentNode = orderedFactorList->head;

    while (currentNode != NULL){
        gmp_printf("%Zd^%Zd ", currentNode->factor->base, currentNode->factor->exponent);
        currentNode = currentNode->next_factor;
    }

*/

#ifdef EXECUTE_TESTS
    numberBuffer_test();
    getInverseMultiplicative_test();
    isInvertible_test();
#endif

    return 0;
}