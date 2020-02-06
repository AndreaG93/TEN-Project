#include "DLogProblemInstance.h"
#include "../Math/Number.h"

DLogProblemInstance *allocateDLogProblemInstance() {

    DLogProblemInstance *output = malloc(sizeof(DLogProblemInstance));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else
        return output;
}