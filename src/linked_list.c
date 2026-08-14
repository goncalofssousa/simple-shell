#include"linked_list.h"
#include<stdlib.h>

List *newList() {
    List *new = malloc(sizeof(List)); 
    new->head = NULL;
    new->tail = NULL; 
    return new; 
}
// return de algo aqui em caso de erro
void listAppend(List *list, void *data){
    ListNode *new = malloc(sizeof(ListNode)); 
    if(!new) return; 
    new->data = data;
    new->next = NULL;    

    if(list->head == NULL){
        list->head = new; 
    } else {
        list->tail->next = new; 
    }

    list->tail = new; 
}

void freeList(List *list, void (*freeData)(void *data)){
    ListNode *before = NULL; 
    
    while(list->head != NULL){
        before = list->head; 
        list->head = list->head->next; 
        freeData(before->data);
        free(before); 
    }
    
    free(list);
}


void printList(List *list, void (*printData)(void *)){
    ListNode *current = list->head;

    while (current) {
        printData(current->data);
        current = current->next;
    }
}