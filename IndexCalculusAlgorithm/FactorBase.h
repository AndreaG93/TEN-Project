#pragma once
#include <gmp.h>

typedef struct {

    __mpz_struct *primeNumber;
    void *next_node;

} FactorBaseNode;

typedef struct {

    __mpz_struct** arrayOfPrime;
    unsigned long length;
    FactorBaseNode *head;
    FactorBaseNode *tail;

} FactorBase;

FactorBase *allocateFactorBase();

void freeFactorBase(FactorBase* input);

void populateFactorBase(FactorBase *factorBase, __mpz_struct *smoothnessBound);