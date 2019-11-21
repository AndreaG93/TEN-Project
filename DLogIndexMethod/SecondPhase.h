#pragma once

#include <gmp.h>
#include "DLogProblemInstance.h"

SecondPhaseOutput* allocateSecondPhaseOutput(unsigned long long size);

void startSecondPhase(DLogProblemInstance *instance);