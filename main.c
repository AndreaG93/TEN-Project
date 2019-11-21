#include "ApplicationBuffer/ApplicationBuffer.h"
#include "Math/OrderedFactorList.h"
#include "Math/Number.h"
#include "DLogIndexMethod/DLogProblemInstance.h"
#include "DLogIndexMethod/ThirdPhase.h"
#include "DLogIndexMethod/FactorBase.h"

//#define EXECUTE_TESTS // Comment to disable tests...
#define AUDIT if(1)

__mpz_struct ***allocateMatrix(unsigned long long row, unsigned long long column) {

    __mpz_struct ***matrix = malloc(column * sizeof(__mpz_struct **));
    if (matrix == NULL)
        exit(EXIT_FAILURE);
    else {
        for (unsigned long long columnIndex = 0; columnIndex < column; columnIndex++) {

            *(matrix + columnIndex) = malloc(row * sizeof(__mpz_struct *));
            __mpz_struct **currentColumn = *(matrix + columnIndex);

            if (currentColumn == NULL)
                exit(EXIT_FAILURE);
            else {

                for (unsigned long long rowIndex = 0; rowIndex < row; rowIndex++) {
                    *(currentColumn + row) = NULL;
                }
            }
        }
    }

    return matrix;
}

void setMatrixNumber(__mpz_struct ***matrix, unsigned long long row, unsigned long long column, __mpz_struct *number) {

    __mpz_struct **targetColumn = *(matrix + column);
    *(targetColumn + row) = number;
}

void printNumberIntoMatrix(__mpz_struct ***matrix, unsigned long long row, unsigned long long column) {

    __mpz_struct **targetColumn = *(matrix + column);
    gmp_printf("%Zd\n", *(targetColumn + row));
}


int main() {

    __mpz_struct* smoothnessBound = allocateAndSetNumberFromULL(15489000);
    FactorBase* factorBase = allocateFactorBase();
    populateFactorBase(factorBase, smoothnessBound);

    gmp_printf("%Zd", factorBase->tail->primeNumber);


    /*
    ApplicationBuffer *applicationBuffer = allocateApplicationBuffer();
    unsigned long long columnSize = 2;
    unsigned long long rowSize = 2;

    __mpz_struct *number1 = allocateAndSetNumberFromULL(12);
    __mpz_struct *number2 = allocateAndSetNumberFromULL(8);
    __mpz_struct *number3 = allocateAndSetNumberFromULL(3);
    __mpz_struct *number4 = allocateAndSetNumberFromULL(2);

    __mpz_struct ***matrix = allocateMatrix(2, 2);
    setMatrixNumber(matrix, 0, 0, number1);
    setMatrixNumber(matrix, 0, 1, number2);
    setMatrixNumber(matrix, 1, 0, number3);
    setMatrixNumber(matrix, 1, 1, number4);


    printNumberIntoMatrix(matrix, 0, 0);
    printNumberIntoMatrix(matrix, 0, 1);
    printNumberIntoMatrix(matrix, 1, 0);
    printNumberIntoMatrix(matrix, 1, 1);


    for (unsigned long long columnIndex = 0; columnIndex < columnSize; columnIndex++) {

    }
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
    ApplicationBuffer *applicationBuffer = allocateApplicationBuffer();

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