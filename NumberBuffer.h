#pragma once

#include <stdlib.h>
#include <gmp.h>

typedef struct _list_elem {
    __mpz_struct *number;
    struct _list_elem *next;
} ChainedHashTableItem;

typedef struct {
    ChainedHashTableItem **table;
    unsigned long long size;
} ChainedHashTable;


unsigned long hash(__mpz_struct *key, unsigned long long hashTableSize, const char* stringBuffer) {

    mpz_get_str(stringBuffer, 10, key);

    const char* s = stringBuffer;
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31*hashval;

    return hashval % hashTableSize;
}

ChainedHashTableItem *allocateChainedHashTableItem(__mpz_struct *number) {

    ChainedHashTableItem *output = malloc(sizeof(ChainedHashTableItem));
    if (output == NULL)
        exit(EXIT_FAILURE);
    else {
        output->number = number;
        output->next = NULL;
        return output;
    }
}


ChainedHashTable *allocateChainedHashTable(int size) {

    ChainedHashTable *output = malloc(sizeof(ChainedHashTable));
    if (output == NULL)
        exit(EXIT_FAILURE);

    output->size = size;
    output->table = malloc(size* sizeof(ChainedHashTableItem *));
    if (output->table == NULL)
        exit(EXIT_FAILURE);
    else
        for (int i = 0; i < size; i++)
            *(output->table + i) = NULL;

    return output;
}

bool chainedHashTableInsert(ChainedHashTable *input, __mpz_struct *number, const char* stringBuffer) {

    unsigned long long index = hash(number, input->size, stringBuffer);

    if (*(input->table + index) == NULL) {
        *(input->table + index) = allocateChainedHashTableItem(number);
        return true;
    } else {

        for (ChainedHashTableItem *currentExistingItem = input->table[index];; currentExistingItem = currentExistingItem->next) {

            if (mpz_cmp(currentExistingItem->number, number) == 0) {
                return false;
            } else if (currentExistingItem->next == NULL) {
                currentExistingItem->next = allocateChainedHashTableItem(number);
                return true;
            }
        }
    }
}


__mpz_struct *chainedHashTableSearch(ChainedHashTable *input, __mpz_struct *number, const char* stringBuffer) {

    unsigned long long index = hash(number, input->size, stringBuffer);

    ChainedHashTableItem *currentExistingItem = input->table[index];

    while (currentExistingItem != NULL) {

        if (mpz_cmp(currentExistingItem->number, number) == 0) {
            return currentExistingItem->number;
        } else {
            currentExistingItem = currentExistingItem->next;
        }
    }

    return NULL;
}



/*

void hash_destroy(ChainedHashTable *T) {
    int i;
    int m = T->size;
    for (i = 0; i < m; i++) {
        list_elem_t *lista = T->table[i];


        while (lista != NULL) {
            list_elem_t *tmp = lista;
            lista = lista->next; // Avanza nella lista
            free(tmp); // Libera il blocco precedente
        }
    }


    free(T->table);

    free(T);
}*/