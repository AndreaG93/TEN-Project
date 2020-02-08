#include "FirstStep.h"

void startFirstStep(DLogProblemInstance *instance) {

    instance->factorBase = allocateFactorBase();
    populateFactorBase(instance->factorBase, instance->smoothnessBound);
}