#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include "DataStructures/UnorderedList.h"
#include "MathCommon_test.h"

#define EXECUTE_TESTS // Comment to disable tests...
#define AUDIT if(1)

int main() {

#ifdef EXECUTE_TESTS
    getInverseMultiplicative_test();
    isInvertible_test();
#endif

    return 0;
}