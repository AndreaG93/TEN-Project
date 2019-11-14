/*
 ============================================================================
 Name        : UnorderedList.h
 Author      : Andrea Graziani
 Version     : 1.0
 Description : ANSI C implementation of an unordered list singly linked list. (HEADER)
 ============================================================================
 */

#ifndef UNTITLED_UNORDEREDLIST_H
#define UNTITLED_UNORDEREDLIST_H

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

struct next_node {
    void *data;
    struct next_node *next;
};

typedef struct next_node UnorderedListNode;

typedef struct {

    unsigned long length;
    UnorderedListNode *head;
    UnorderedListNode *tail;

} UnorderedList;

UnorderedList *allocUnorderedList();
void insert(UnorderedList *list, void *data);
void performAction(UnorderedList *list, void(*function)(void *));
void *get(UnorderedList *list, int index);

#endif //UNTITLED_UNORDEREDLIST_H
