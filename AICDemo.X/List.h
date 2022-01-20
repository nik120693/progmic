#ifndef BASE_LINKED_LIST__LIST_H_
#define BASE_LINKED_LIST__LIST_H_

typedef struct node {
  int data;
  struct node *before;
  struct node *next;
  struct node *tail;
} Node;

int add_EventLog(Node **head, int data);
void rm_Logs(Node **head);
void rm_List(Node **head);
Node *init_list();
void index_List(int num, int newLine);

#endif //BASE_LINKED_LIST__LIST_H_
