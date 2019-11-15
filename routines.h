//
// Created by Andrea on 14/11/2019.
//

#ifndef TEN_PROJECT_ROUTINES_H
#define TEN_PROJECT_ROUTINES_H

#include "DataStructures/UnorderedList.h"

typedef UnorderedList FactorBase;
typedef UnorderedList FactorList;

FactorBase *computeFactorBase(mpz_ptr smoothnessBound);
mpz_t *getMultiplicativeInverseWithExtendedEuclideanAlgorithm(mpz_t input, mpz_t modulo);

#endif //TEN_PROJECT_ROUTINES_H
