#include <stdlib.h>
#include <stdio.h>
#include "OrderedFactorList.h"
#include "Number.h"

OrderedFactorList *allocateOrderedFactorList() {

    OrderedFactorList *output = calloc(1, sizeof(OrderedFactorList));
    if (output == NULL) {
        exit(EXIT_FAILURE);
    } else {
        output->head = NULL;
        output->tail = NULL;
    }

    return output;
}

OrderedFactorListNode *allocateOrderedFactorListNode(__mpz_struct *primeNumber, __mpz_struct *primeNumberExponent) {

    OrderedFactorListNode *output = malloc(sizeof(OrderedFactorListNode));
    if (output == NULL) {
        exit(EXIT_FAILURE);
    } else {

        Factor *newFactor = allocateFactor(primeNumber, primeNumberExponent);

        output->factor = newFactor;
        output->next_node = NULL;

        return output;
    }
}

void appendFactor(OrderedFactorList *list, __mpz_struct *primeNumber, __mpz_struct *primeNumberExponent) {

    OrderedFactorListNode* node = allocateOrderedFactorListNode(primeNumber, primeNumberExponent);

    if (list->head == NULL) {
        list->head = node;
        list->tail = list->head;
    } else {
        list->tail->next_node = node;
        list->tail = node;
    }
}

void insertFactor(OrderedFactorList *list, __mpz_struct *primeNumber, __mpz_struct *primeNumberExponent) {

    OrderedFactorListNode *previousNode = list->head;
    OrderedFactorListNode *currentNode = list->head;

    if (list->head == NULL) {
        list->head = allocateOrderedFactorListNode(primeNumber, primeNumberExponent);
        list->tail = list->head;
    } else {
        while (currentNode != NULL) {

            int comparison = mpz_cmp(primeNumber, currentNode->factor->base);

            if (comparison == 0) {

                mpz_add(currentNode->factor->exponent, currentNode->factor->exponent, primeNumberExponent);
                deallocateNumber(primeNumber);
                deallocateNumber(primeNumberExponent);

                return;

            } else if (comparison < 0) {

                OrderedFactorListNode *newNode = allocateOrderedFactorListNode(primeNumber, primeNumberExponent);

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

        appendFactor(list, primeNumber, primeNumberExponent);
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

void printOrderedFactorList(OrderedFactorList *list) {

    OrderedFactorListNode *currentNode = list->head;

    while (true) {

        if (currentNode->next_node != NULL) {
            gmp_fprintf(stderr, "[ %Zd ^ %Zd ] * ", currentNode->factor->base, currentNode->factor->exponent);
            currentNode = currentNode->next_node;
        } else {
            gmp_fprintf(stderr, "[ %Zd ^ %Zd ]", currentNode->factor->base, currentNode->factor->exponent);
            break;
        }
    }
}

OrderedFactorList *mergeOrderedFactorListUsingOptimization(OrderedFactorList *listA, OrderedFactorList *listB) {

    OrderedFactorList *output = allocateOrderedFactorList();

    OrderedFactorListNode *nodeListA = listA->head;
    OrderedFactorListNode *nodeListB = listB->head;

    while (nodeListA != NULL || nodeListB != NULL) {

        __mpz_struct *base = allocateNumber();
        __mpz_struct *exponent = allocateNumber();

        if (nodeListA != NULL && nodeListB != NULL) {
            if (mpz_cmp(nodeListA->factor->base, nodeListB->factor->base) == 0) {

                mpz_set(base, nodeListA->factor->base);
                mpz_sub(exponent, nodeListA->factor->exponent, nodeListB->factor->exponent);

                insertFactor(output, base, exponent);

                nodeListA = nodeListA->next_node;
                nodeListB = nodeListB->next_node;

                continue;
            }
        }

        if (nodeListA != NULL) {

            mpz_set(base, nodeListA->factor->base);
            mpz_set(exponent, nodeListA->factor->exponent);

            insertFactor(output, base, exponent);

            nodeListA = nodeListA->next_node;

            continue;
        }

        if (nodeListB != NULL) {

            mpz_set(base, nodeListB->factor->base);
            mpz_mul_si(exponent, nodeListB->factor->exponent, -1);

            insertFactor(output, base, exponent);

            nodeListB = nodeListB->next_node;

            continue;
        }
    }

    return output;
}