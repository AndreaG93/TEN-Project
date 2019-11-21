#pragma once

#include <gmp.h>

typedef struct {

    __mpz_struct *primeNumber;
    void *next_node;

} FactorBaseNode;

typedef struct {

    unsigned long length;
    FactorBaseNode *head;
    FactorBaseNode *tail;

} FactorBase;

FactorBase *allocateFactorBase();

void populateFactorBase(FactorBase *factorBase, __mpz_struct *smoothnessBound);