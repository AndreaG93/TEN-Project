#include "Math/OrderedFactorList.h"
#include "Math/Number.h"
#include "DLogIndexMethod/DLogProblemInstance.h"
#include "DLogIndexMethod/ThirdPhase.h"
#include "DLogIndexMethod/FactorBase.h"
#include "DLogIndexMethod/FirstPhase.h"
#include "DLogIndexMethod/SecondPhase.h"
#include "Math/Common.h"
#include "DLogIndexMethod/RelationsRetrieval.h"
#include "ThreadsPool/ThreadsPool.h"
#include "Math/Matrix.h"
#include "Math/Factorization.h"

#define MAX_RANDOM_INTEGER 20

//#define EXECUTE_TESTS // Comment to disable tests...
#define AUDIT if(1)


int main() {

/*
    ApplicationBuffer *applicationBuffer = allocateApplicationBuffer();

  __mpz_struct *number = allocateAndSetNumberFromULL(1);
    __mpz_struct *bound = allocateAndSetNumberFromULL(7);

    OrderedFactorList *orderedFactorList = factorizeCheckingBSmoothness(applicationBuffer, number, bound);
    OrderedFactorListNode *currentNode = orderedFactorList->head;

    while (currentNode != NULL){
        gmp_printf("%Zd^%Zd ", currentNode->factor->base, currentNode->factor->exponent);
        currentNode = currentNode->next_node;
    }

*/



    DLogProblemInstance *instance = allocateDLogProblemInstance("179", "2", "13", MAX_RANDOM_INTEGER);
    setSmoothnessBound(instance, "7");


    instance->threadsPoolData = allocateThreadsPoolData(instance);

    startThreadsPool(1, &threadRoutineForRelationRetrieval, (void *) instance->threadsPoolData);

    startFirstPhase(instance);
    startSecondPhase(instance);


    for (unsigned long index = 0; index < instance->factorBase->length; index++) {
        gmp_printf("-> %Zd \n", *(instance->secondPhaseOutput->solution + index));
    }

    startThirdPhase(instance);


/*
    __mpz_struct *modulo = allocateAndSetNumberFromULL(178);

    __mpz_struct *number1 = allocateAndSetNumberFromString("0");
    __mpz_struct *number2 = allocateAndSetNumberFromString("2");
    __mpz_struct *number3 = allocateAndSetNumberFromString("1");
    __mpz_struct *number4 = allocateAndSetNumberFromString("0");

    __mpz_struct *number5 = allocateAndSetNumberFromString("0");
    __mpz_struct *number6 = allocateAndSetNumberFromString("-1");
    __mpz_struct *number7 = allocateAndSetNumberFromString("2");
    __mpz_struct *number8 = allocateAndSetNumberFromString("-1");

    __mpz_struct *number9 = allocateAndSetNumberFromString("0");
    __mpz_struct *number10 = allocateAndSetNumberFromString("-2");
    __mpz_struct *number11 = allocateAndSetNumberFromString("-1");
    __mpz_struct *number12 = allocateAndSetNumberFromString("1");

    __mpz_struct *number13 = allocateAndSetNumberFromString("-2");
    __mpz_struct *number14 = allocateAndSetNumberFromString("-3");
    __mpz_struct *number15 = allocateAndSetNumberFromString("-5");


    Matrix *matrix = allocateMatrix(3, 5);

    setNumberIntoMatrixCell(matrix, 0, 0, number1);
    setNumberIntoMatrixCell(matrix, 0, 1, number2);
    setNumberIntoMatrixCell(matrix, 0, 2, number3);
    setNumberIntoMatrixCell(matrix, 0, 3, number4);
    setNumberIntoMatrixCell(matrix, 1, 0, number5);
    setNumberIntoMatrixCell(matrix, 1, 1, number6);
    setNumberIntoMatrixCell(matrix, 1, 2, number7);
    setNumberIntoMatrixCell(matrix, 1, 3, number8);
    setNumberIntoMatrixCell(matrix, 2, 0, number9);
    setNumberIntoMatrixCell(matrix, 2, 1, number10);
    setNumberIntoMatrixCell(matrix, 2, 2, number11);
    setNumberIntoMatrixCell(matrix, 2, 3, number12);

    setNumberIntoMatrixCell(matrix, 0, 4, number13);
    setNumberIntoMatrixCell(matrix, 1, 4, number14);
    setNumberIntoMatrixCell(matrix, 2, 4, number15);


    printMatrix(matrix);



*/



/*
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
*/



/*
#ifdef EXECUTE_TESTS
    numberBuffer_test();
    getInverseMultiplicative_test();
    isInvertible_test();
#endif
*/
    return 0;
}