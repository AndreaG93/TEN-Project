#include "FirstPhase.h"

void startFirstPhase(DLogProblemInstance* instance){

    instance->factorBase = allocateFactorBase();
    populateFactorBase(instance->factorBase, instance->smoothnessBound);
}

