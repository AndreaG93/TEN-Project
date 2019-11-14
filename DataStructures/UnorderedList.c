/*
 ============================================================================
 Name        : UnorderedList.c
 Author      : Andrea Graziani
 Version     : 1.0
 Description : ANSI C implementation of an unordered list singly linked list.
 ============================================================================
 */

#include "UnorderedList.h"


UnorderedList *allocUnorderedList() {
    UnorderedList *output = malloc(sizeof(UnorderedList));
    if (output == NULL) {
        perror("'allocUnorderedList' --> malloc");
        exit(EXIT_FAILURE);
    }

    output->head = NULL;
    output->length = 0;

    return output;
}

UnorderedListNode *allocUnorderedListNode(void *data) {
    UnorderedListNode *output = malloc(sizeof(UnorderedListNode));
    if (output == NULL) {
        perror("'allocUnorderedListNode' --> malloc");
        exit(EXIT_FAILURE);
    }

    output->data = data;
    output->next = NULL;

    return output;
}

void insert(UnorderedList *list, void *data) {

    UnorderedListNode *newNode = allocUnorderedListNode(data);

    if (list->length == 0) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }

    list->length++;
}

void performAction(UnorderedList *list, void(*function)(void *)) {

    for (UnorderedListNode *current = list->head; current != NULL; current = current->next) {
        function(current->data);
    }
}

void *get(UnorderedList *list, int index) {

    int i = 0;

    if (index > (list->length - 1))
        return NULL;

    for (UnorderedListNode *current_node = list->head; current_node != NULL; current_node = current_node->next, i++) {
        if (i == index)
            return current_node->data;
    }

    return NULL;
}

/*
void delete(UnorderedList *p_list, unsigned int p_index) {

	UnorderedListNode *current = p_list->head;
	UnorderedListNode *previous = NULL;

	for (unsigned int i = 0; current != NULL; i++) {

		if (p_index == i) {

			if (p_list->length == 1)
			{
				p_list->head = NULL;
				p_list->tail = NULL;
			}
			else if (current == p_list->head) {
				p_list->head = current->next;
			}
			else {
				previous->next = current->next;
			}

			free(current);
			p_list->length--;
		}

		previous->next = current->next;
		current = current->next;
	}
}
*/