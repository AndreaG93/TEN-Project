#include <stdlib.h>
#include "OrderedFactorList.h"
#include "Number.h"

OrderedFactorList *allocateOrderedFactorList() {

    OrderedFactorList *output = malloc(sizeof(OrderedFactorList));

    if (output == NULL) {
        exit(EXIT_FAILURE);
    } else {
        output->head = NULL;

        return output;
    }
}

OrderedFactorListNode *allocateOrderedFactorListNode(void *factor) {

    Factor *newFactor = allocateFactor();
    newFactor->base = factor;
    newFactor->exponent = allocateAndSetNumberFromULL(1);

    OrderedFactorListNode *output = malloc(sizeof(OrderedFactorListNode));
    if (output == NULL) {
        exit(EXIT_FAILURE);
    } else {

        output->factor = newFactor;
        output->next_node = NULL;

        return output;
    }
}

void insertNewFactor(OrderedFactorList *list, __mpz_struct *newBaseFactor) {

    OrderedFactorListNode *previousNode = list->head;
    OrderedFactorListNode *currentNode = list->head;

    if (list->head == NULL) {
        list->head = allocateOrderedFactorListNode(newBaseFactor);
        list->tail = list->head;
    } else {
        while (currentNode != NULL) {

            int comparison = mpz_cmp(newBaseFactor, currentNode->factor->base);

            if (comparison == 0) {
                mpz_add_ui(currentNode->factor->exponent, currentNode->factor->exponent, 1);
                mpz_clear(newBaseFactor);
                return;

            } else if (comparison < 0) {

                OrderedFactorListNode *newNode = allocateOrderedFactorListNode(newBaseFactor);

                if (list->head == list->tail)
                    list->head = newNode;
                else
                    previousNode->next_node = newNode;

                newNode->next_node = currentNode;
                return;

            } else {
                previousNode = currentNode;
                currentNode = currentNode->next_node;
            }
        }

        currentNode = allocateOrderedFactorListNode(newBaseFactor);

        previousNode->next_node = currentNode;
        list->tail = currentNode;
    }
}

void deallocateOrderedFactorList(OrderedFactorList *list) {

    OrderedFactorListNode *currentNode = list->head;
    OrderedFactorListNode *nextNode;
    Factor *currentFactor;

    while (currentNode != NULL) {

        nextNode = currentNode->next_node;
        currentFactor = currentNode->factor;

        mpz_clear(currentFactor->base);
        mpz_clear(currentFactor->exponent);

        free(currentFactor);
        free(currentNode);

        currentNode = nextNode;
    }

    free(list);
}