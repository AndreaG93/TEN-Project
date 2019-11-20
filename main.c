#include "ApplicationBuffer/ApplicationBuffer.h"
#include "Math/OrderedFactorList.h"
#include "Math/Number.h"
#include "DLogIndexMethod/DLogProblemInstance.h"
#include "DLogIndexMethod/ThirdPhase.h"

//#define EXECUTE_TESTS // Comment to disable tests...
#define AUDIT if(1)


int main() {

    DLogProblemInstance *instance = allocateDLogProblemInstance("179", "2", "13");

    instance->primitiveRoot = allocateAndSetNumberFromString("2");
    instance->factorBaseLength = 4;

    mpz_set_ui(*(instance->factorBase), 2);
    mpz_set_ui(*(instance->factorBase + 1), 3);
    mpz_set_ui(*(instance->factorBase + 2), 5);
    mpz_set_ui(*(instance->factorBase + 3), 7);

    mpz_set_ui(*(instance->secondPhaseOutput->solution), 1);
    mpz_set_ui(*(instance->secondPhaseOutput->solution + 1), 108);
    mpz_set_ui(*(instance->secondPhaseOutput->solution + 2), 138);
    mpz_set_ui(*(instance->secondPhaseOutput->solution + 3), 171);

    instance->smoothnessBound = allocateAndSetNumberFromULL(7);

    startThirdPhase(instance);


/*
    ApplicationBuffer *applicationBuffer = allocateApplicationBuffer();

  __mpz_struct *number = allocateAndSetNumberFromULL(7242342342352);

    OrderedFactorList *orderedFactorList = factorize(applicationBuffer, number);
    OrderedFactorListNode *currentNode = orderedFactorList->head;

    while (currentNode != NULL){
        gmp_printf("%Zd^%Zd ", currentNode->factor->base, currentNode->factor->exponent);
        currentNode = currentNode->next_node;
    }

*/

#ifdef EXECUTE_TESTS
    numberBuffer_test();
    getInverseMultiplicative_test();
    isInvertible_test();
#endif

    return 0;
}