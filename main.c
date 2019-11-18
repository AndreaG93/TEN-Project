#include "MathCommon_test.h"
#include "NumberBuffer_test.h"

#define EXECUTE_TESTS // Comment to disable tests...
#define AUDIT if(1)



int main() {

    initializeControlBlock();

#ifdef EXECUTE_TESTS
    numberBuffer_test();
    getInverseMultiplicative_test();
    isInvertible_test();
#endif

    return 0;
}