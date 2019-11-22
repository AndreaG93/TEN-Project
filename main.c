#include "Math/OrderedFactorList.h"
#include "Math/Number.h"
#include "DLogIndexMethod/DLogProblemInstance.h"
#include "DLogIndexMethod/ThirdPhase.h"
#include "DLogIndexMethod/FactorBase.h"
#include "DLogIndexMethod/FirstPhase.h"
#include "DLogIndexMethod/SecondPhase.h"
#include "Math/Common.h"

#define MAX_RANDOM_INTEGER 20

//#define EXECUTE_TESTS // Comment to disable tests...
#define AUDIT if(1)








int main() {


    DLogProblemInstance *instance = allocateDLogProblemInstance("179", "2", "13");
    setSmoothnessBound(instance, "7");
    initializeRandIntegerGenerator(instance, MAX_RANDOM_INTEGER);
    /*
       startFirstPhase(instance);
       startSecondPhase(instance);

       //todo to cancel
       mpz_set_ui(*(instance->secondPhaseOutput->solution), 1);
       mpz_set_ui(*(instance->secondPhaseOutput->solution + 1), 108);
       mpz_set_ui(*(instance->secondPhaseOutput->solution + 2), 138);
       mpz_set_ui(*(instance->secondPhaseOutput->solution + 3), 171);
       //todo to cancel

       startThirdPhase(instance);
   */

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

    setMatrixNumber(matrix, 0, 0, number1);
    setMatrixNumber(matrix, 0, 1, number2);
    setMatrixNumber(matrix, 0, 2, number3);
    setMatrixNumber(matrix, 0, 3, number4);
    setMatrixNumber(matrix, 1, 0, number5);
    setMatrixNumber(matrix, 1, 1, number6);
    setMatrixNumber(matrix, 1, 2, number7);
    setMatrixNumber(matrix, 1, 3, number8);
    setMatrixNumber(matrix, 2, 0, number9);
    setMatrixNumber(matrix, 2, 1, number10);
    setMatrixNumber(matrix, 2, 2, number11);
    setMatrixNumber(matrix, 2, 3, number12);

    setMatrixNumber(matrix, 0, 4, number13);
    setMatrixNumber(matrix, 1, 4, number14);
    setMatrixNumber(matrix, 2, 4, number15);


    printMatrix(matrix);

    gauss(matrix, instance->applicationBuffer, modulo);
    //gauss(matrix, instance->applicationBuffer, modulo);

    fprintf(stdout, "===\n");

    printMatrix(matrix);

*/


/*


    for (
            k = 0;
            k < n - 1; k++) {

        if (a[k][k] == 0) {
            printf("Can't solve this linear equations by Gaussian-elimination.\n");
            return 1;
        }
        for (
                i = k + 1;
                i < n;
                i++) {
            l[i][k] = a[i][k] / a[k][k];
            c++;
        }
        for (
                i = k + 1;
                i < n;
                i++) {
            for (
                    j = k + 1;
                    j < n;
                    j++) {
                a[i][j] = a[i][j] - l[i][k] * a[k][j];
                c++;
            }
            b[i] = b[i] - l[i][k] * b[k];
            c++;
            a[i][k] = 0;
        }
    }

    printf("After converting, A(%d*%d) and B(%d*1) can be:\n", n, n, n);
    for (
            i = 0;
            i < n;
            i++) {
        for (
                j = 0;
                j < n;
                j++) {
            printf("%.2f  ", a[i][j]);
        }
        printf("%.2f\n", b[i]);
    }

    printf("The solution to x is: ");
    x[n - 1] = b[n - 1] / a[n - 1][n - 1];
    c++;
    for (
            i = n - 2;
            i >= 0; i--) {
        result = 0.0;
        for (
                j = i + 1;
                j < n;
                j++) {
            result = result + a[i][j] * x[j];
            c++;
        }
        x[i] = (b[i] - result) / a[i][i];
        c++;
    }

    for (
            i = 0;
            i < n;
            i++) {
        printf("%.2f  ", x[i]);
    }
    printf("\nThe number of times in multiplication and division is: %d\n", c);



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
    Buffers *applicationBuffer = allocateApplicationBuffer();

  __mpz_struct *number = allocateAndSetNumberFromULL(7242342342352);

    OrderedFactorList *orderedFactorList = factorize(applicationBuffer, number);
    OrderedFactorListNode *currentNode = orderedFactorList->head;

    while (currentNode != NULL){
        gmp_printf("%Zd^%Zd ", currentNode->factor->base, currentNode->factor->exponent);
        currentNode = currentNode->next_node;
    }

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