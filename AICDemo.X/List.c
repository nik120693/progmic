#include <stdlib.h>
#include "list.h"
#include "uart.h"

int add_EventLog(Node **head, int data) {
    int executed = 0;
  if ((*head)->data == -1)
    (*head)->data = data;
  else {
    Node* current = (*head);
    while(current->next != NULL)
        current = current->next;
      
    Node *new_node = (Node *) malloc(sizeof(Node));
    new_node->data = data;
    new_node->before = current;
    new_node->next = NULL;
    
    current->next = new_node;
    (*head)->tail = new_node;
  }
    executed = 1;
    return executed;
}
void index_List(int num, int newLine){
    Node* values = init_list();
     
    while(num > 0) {
        int mod = num % 10;
        add_EventLog(&values, mod);
        num /= 10;
    }
    
    Node* current = values->tail;
    while(current != NULL) {
        putU4(current->data + 48); // digit + ascii code for 0
        current = current->before;
    }
    
    rm_List(&values);
    if (newLine == 1) {
       putU4_string("\r");;
    }
 }

void rm_Logs(Node **head) {
  Node *current = (*head)->next;
  Node *next;
  (*head)->data = -1;
  (*head)->before = NULL;
  (*head)->next = NULL;
  (*head)->tail = NULL;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
}

void rm_List(Node **head) {
  rm_Logs(head);
  free(*head);
}

Node *init_list() {
  Node *head = (Node *) malloc(sizeof(Node));
  head->data = -1;
  head->before = NULL;
  head->next = NULL;
  head->tail = head;
  return head;
}
