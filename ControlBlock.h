#pragma once

#include <stdlib.h>
#include "NumberBuffer.h"



typedef struct {

    char *stringBuffer;
    ChainedHashTable *mainNumberBuffer;
    __mpz_struct **auxiliaryNumberBuffer;

} ControlBlock;

ControlBlock *controlBlock;

void initializeControlBlock() {

    controlBlock = malloc(sizeof(ControlBlock));
    if (controlBlock == NULL)
        exit(EXIT_FAILURE);
    else {

        controlBlock->stringBuffer = calloc(40, sizeof(char));
        if (controlBlock->stringBuffer == NULL)
            exit(EXIT_FAILURE);

        controlBlock->mainNumberBuffer = allocateChainedHashTable(100000000);
        if (controlBlock->mainNumberBuffer == NULL)
            exit(EXIT_FAILURE);

        controlBlock->auxiliaryNumberBuffer = allocateAuxiliaryBuffer();
        if (controlBlock->auxiliaryNumberBuffer == NULL)
            exit(EXIT_FAILURE);
    }
}