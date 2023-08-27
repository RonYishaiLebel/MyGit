#include "linked_list.h"

/**
 * Add a new node with data to the end of the linked list.
 *
 * This function creates a new node and adds it to the end of the linked list.
 *
 * @param link_list A pointer to the linked list.
 * @param data The data to be stored in the new node.
 * @return Returns 0 on success, or 1 if memory allocation fails.
 */
int add(LinkedList *link_list, void *data)
{
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL)
    {
        return 1;
    }
    *new_node = (Node) {data, NULL};

    if (link_list->first == NULL)
    {
        link_list->first = new_node;
        link_list->last = new_node;
    }
    else
    {
        link_list->last->next = new_node;
        link_list->last = new_node;
    }

    link_list->size++;
    return 0;
}
