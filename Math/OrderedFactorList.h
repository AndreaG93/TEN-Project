#pragma once

#include "Factor.h"

typedef struct ordered_factor_list_node {

    Factor *factor;
    struct ordered_factor_list_node *next_node;

} OrderedFactorListNode;

typedef struct {

    OrderedFactorListNode *head;
    OrderedFactorListNode *tail;

} OrderedFactorList;

OrderedFactorList *allocateOrderedFactorList();

void deallocateOrderedFactorList(OrderedFactorList *list);

void insertNewFactor(OrderedFactorList *list, __mpz_struct *newBaseFactor);