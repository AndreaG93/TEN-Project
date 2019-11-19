#pragma once

#include "Factor.h"

typedef struct factor_node {

    Factor *factor;
    struct factor_node *next_factor;

} OrderedFactorListNode;

typedef struct {

    unsigned long length;
    OrderedFactorListNode *head;

} OrderedFactorList;

OrderedFactorList *allocateOrderedFactorList();

void insertNewFactor(OrderedFactorList *list, __mpz_struct *newBaseFactor);