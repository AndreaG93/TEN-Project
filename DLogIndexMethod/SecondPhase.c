//
// Created by andrea on 20/11/19.
//

#include <stdlib.h>
#include "SecondPhase.h"
#include "../Math/Number.h"

SecondPhaseOutput* allocateSecondPhaseOutput(unsigned long long size){

    SecondPhaseOutput *output = malloc(sizeof(SecondPhaseOutput));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->solution = allocateNumbersArray(size);
    }

    return output;
}