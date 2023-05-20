#ifndef __LIST_H__
#define __LIST_H__

struct List
{
    void *value;
    struct List *next;
};
typedef struct List List;

List *createNode(void *value, List *next);

List *pushNode(List *head, List *node);
List *insertNode(List *head, List *node);

int listLength(List *head);

void detruireNode(List *node);

#endif