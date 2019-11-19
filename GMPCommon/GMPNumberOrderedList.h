#pragma once

#include <gmp.h>

typedef struct next_node {

    void *data;
    struct next_node *next;

} GMPNumberOrderedListNode;

typedef struct {

    unsigned long length;
    GMPNumberOrderedListNode *head;

} GMPNumberOrderedList;

GMPNumberOrderedList *allocateGMPNumberOrderedList();

void insert(GMPNumberOrderedList *list, __mpz_struct *data);