#pragma once

typedef struct {

    void *tail;
    void *head;

} SemiLockFreeQueue;

SemiLockFreeQueue *allocateSemiLockFreeQueue();

void enqueue(SemiLockFreeQueue *queue, void *data);

void *dequeue(SemiLockFreeQueue *queue);

void freeSemiLockFreeQueue(SemiLockFreeQueue *queue, void (*freeDataFunction)(void *));
