#include "list.h"

#include <stdio.h>
#include <stdlib.h>

List *createNode(void *value, List *next)
{
    List *p = (List *)malloc(sizeof(List));

    p->value = value;
    p->next = next;

    return p;
}

List *pushNode(List *head, List *node)
{
    List *p = head;

    node->next = NULL;

    if (head == NULL || p == NULL)
        return node;

    while (p->next != NULL)
        p = p->next;

    p->next = node;

    return head;
}

List *insertNode(List *head, List *node)
{
    node->next = head;

    return node;
}

void detruireNode(List *node)
{
    free(node);
}

int listLength(List *head)
{
    int result = 0;

    while (head != NULL)
    {
        result++;
        head = head->next;
    }

    return result;
}