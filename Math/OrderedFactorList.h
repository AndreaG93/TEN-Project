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

void appendFactor(OrderedFactorList *list, __mpz_struct *primeNumber, __mpz_struct *primeNumberExponent);

void insertFactor(OrderedFactorList *list, __mpz_struct *primeNumber, __mpz_struct *primeNumberExponent);

void printOrderedFactorList(OrderedFactorList *list);

OrderedFactorList * mergeOrderedFactorListUsingOptimization(OrderedFactorList *listA, OrderedFactorList *listB);