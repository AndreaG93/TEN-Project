#include <stdlib.h>
#include "Factor.h"

Factor *allocateFactor() {

    Factor *output = malloc(sizeof(Factor));

    if (output == NULL) {
        exit(EXIT_FAILURE);
    } else
        return output;
}