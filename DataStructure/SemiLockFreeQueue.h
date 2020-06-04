#pragma once

typedef struct {

    void *tail __attribute__((aligned(64)));;
    void *head __attribute__((aligned(64)));;

} SemiLockFreeQueue __attribute__((aligned(64)));;

SemiLockFreeQueue *allocateSemiLockFreeQueue();

void enqueue(SemiLockFreeQueue *queue, void *data);

void *dequeue(SemiLockFreeQueue *queue);

void freeSemiLockFreeQueue(SemiLockFreeQueue *queue, void (*freeDataFunction)(void *));
