#include <stdlib.h>
#include "OrderedFactorList.h"
#include "Number.h"

OrderedFactorList *allocateOrderedFactorList() {

    OrderedFactorList *output = malloc(sizeof(OrderedFactorList));

    if (output == NULL) {
        exit(EXIT_FAILURE);
    } else {
        output->head = NULL;
        output->length = 0;

        return output;
    }
}

OrderedFactorListNode *allocateOrderedFactorListNode(void *factor) {

    Factor *newFactor = allocateFactor();
    newFactor->base = factor;
    newFactor->exponent = allocateAndSetULLNumber(1);

    OrderedFactorListNode *output = malloc(sizeof(OrderedFactorListNode));
    if (output == NULL) {
        exit(EXIT_FAILURE);
    } else {

        output->factor = newFactor;
        output->next_factor = NULL;

        return output;
    }
}

void insertNewFactor(OrderedFactorList *list, __mpz_struct *newBaseFactor) {

    OrderedFactorListNode *previousNode = list->head;
    OrderedFactorListNode *currentNode = list->head;

    if (list->head == NULL) {
        list->head = allocateOrderedFactorListNode(newBaseFactor);
    } else {
        while (currentNode != NULL) {

            int comparison = mpz_cmp(newBaseFactor, currentNode->factor->base);

            if (comparison == 0) {
                mpz_add_ui(currentNode->factor->exponent, currentNode->factor->exponent, 1);
                mpz_clear(newBaseFactor);
                return;

            } else if (comparison < 0) {

                OrderedFactorListNode *newNode = allocateOrderedFactorListNode(newBaseFactor);

                if (list->length == 1)
                    list->head = newNode;
                else
                    previousNode->next_factor = newNode;

                newNode->next_factor = currentNode;

                list->length++;
                return;
            } else {
                previousNode = currentNode;
                currentNode = currentNode->next_factor;
            }
        }

        previousNode->next_factor = allocateOrderedFactorListNode(newBaseFactor);
    }

    list->length++;
}