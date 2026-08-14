#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct listNode {
    void *data; 
    struct listNode *next; 
} ListNode;

typedef struct list {
    ListNode *head; 
    ListNode *tail;
} List; 

List *newList();
void listAppend(List *list, void *data); 
void freeList(List *list, void (*freeData)(void *data)); 
void printList(List *list, void (*printData)(void *data)); 

#endif