#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SemiLockFreeQueue.h"
#include "../Common/SynchronizationPrimitives.h"

typedef struct {

    void *next;
    void *data;
    unsigned long dataSize;

} SemiLockFreeQueueNode;

SemiLockFreeQueueNode *allocateSemiLockFreeQueueNode(void *data) {

    SemiLockFreeQueueNode *output = malloc(sizeof(SemiLockFreeQueueNode));
    if (output == NULL)
        exit(EXIT_FAILURE);

    output->data = data;
    output->next = NULL;

    return output;
}

SemiLockFreeQueue *allocateSemiLockFreeQueue() {

    SemiLockFreeQueue *output;
    SemiLockFreeQueueNode *outputDummyNode;

    output = malloc(sizeof(SemiLockFreeQueue));
    if (output == NULL)
        exit(EXIT_FAILURE);

    outputDummyNode = allocateSemiLockFreeQueueNode(NULL);
    if (outputDummyNode == NULL)
        exit(EXIT_FAILURE);

    output->head = outputDummyNode;
    output->tail = outputDummyNode;

    return output;
}

void freeSemiLockFreeQueue(SemiLockFreeQueue *queue, void (*freeFunction)(void *)) {

    SemiLockFreeQueueNode *currentNode;
    SemiLockFreeQueueNode *nextNode;

    for (currentNode = queue->head; currentNode != NULL;) {

        nextNode = currentNode->next;

        if (currentNode->data != NULL)
            (*freeFunction)(currentNode->data);

        free(currentNode);
        currentNode = nextNode;
    }

    free(queue);
}

void enqueue(SemiLockFreeQueue *queue, void *data) {

    SemiLockFreeQueueNode *actualTail;
    SemiLockFreeQueueNode *newNode;

    newNode = allocateSemiLockFreeQueueNode(data);

    while (true) {

        actualTail = queue->tail;
        if (COMPARE_AND_SWAP(&queue->tail, actualTail, newNode)) {
            actualTail->next = newNode;
            break;
        }
    }
}

void *dequeue(SemiLockFreeQueue *queue) {

    SemiLockFreeQueueNode *actualHead;
    void *output;

    do {

        actualHead = queue->head;

    } while (actualHead == NULL || !COMPARE_AND_SWAP(&queue->head, actualHead, NULL));

    if (actualHead->next == NULL) {
        queue->head = actualHead;
        return NULL;
    }

    output = ((SemiLockFreeQueueNode *) actualHead->next)->data;
    ((SemiLockFreeQueueNode *) actualHead->next)->data = NULL;

    queue->head = actualHead->next;
    free(actualHead);

    return output;
}