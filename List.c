//-----------------------------------------------------------------------------
// List.c
// Doubly-linked List ADT
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include "List.h"

// structs --------------------------------------------------------------------

// private nodeobj type
typedef struct NodeObj
{
  void* data;
  struct NodeObj* next;
  struct NodeObj* prev;
} NodeObj;


// private node type
typedef NodeObj* Node;

// private listobj type
typedef struct ListObj{
    Node front;
    Node back;
    Node cursor;
    int index;
    int length;
} ListObj;

// constructors & destructors --------------------------------------------------------------------
Node newNode(void* data)
{
  Node N = malloc(sizeof(NodeObj));
  assert(N != NULL);
  N->data = data;
  N->next = NULL;
  N->prev = NULL;
  return N;
}

void freeNode(Node* pN)
{
   if( pN!=NULL && *pN!=NULL )
  {
      free(*pN);
      *pN = NULL;
  }
}

List newList(void)
{
  List L = malloc(sizeof(ListObj));
  L->front = L->back = L->cursor = NULL;
  L->length = 0;
  return L;
}

void freeList(List *pL)
{
  if (pL != NULL && *pL != NULL)
  {
    while (length(*pL) != 0)
    {
      deleteFront(*pL);
    }
    free(*pL);
    *pL = NULL;
  }
}


int length(List L)
{
  if (L == NULL)
  {
    printf("List Error: calling length() on NULL List reference\n");
    exit(1);
  }
  return L->length;
}

int index(List L)
{
  if (L == NULL)
  {
    printf("List Error: calling index() on NULL List reference\n");
    exit(1);
  }
  if (L->cursor == NULL)
  {
    return -1;
  }
  return L->index;
}

void* front(List L)
{
  if (L == NULL)
  {
    printf("List Error: calling front() on NULL List reference\n");
    exit(1);
  }
  if (L->length == 0 || L->front == NULL)
  {
    printf("List Error: calling front() on an empty List\n");
    exit(1);
  }
  return L->front->data;
}

void* back(List L)
{
  if (L == NULL)
  {
    printf("List Error: calling back() on NULL List reference\n");
    exit(1);
  }
  if (L->length == 0 || L->back == NULL)
  {
    printf("List Error: calling back() on an empty List\n");
    exit(1);
  }
  return L->back->data;
}

void* get(List L)
{
  if (L == NULL)
  {
    printf("List Error: calling get() on NULL List reference\n");
    exit(1);
  }
  if (L->cursor == NULL)
  {
    printf("List Error: calling get() on a NULL cursor\n");
    exit(1);
  }
  return L->cursor->data;
}

/***
bool equals(List A, List B)
{
  if (A == NULL || B == NULL)
  {
    printf("List Error: calling equals() on NULL List reference\n");
    exit(1);
  }
  bool is_equal = false;
  Node N = NULL;
  Node M = NULL;

  if (A->length == B->length)
  {
    is_equal = true;
    N = A->front;
    M = B->front;
    while (is_equal && N != NULL)
    { // ends if ever not same
      is_equal = (N->data == M->data);
      N = N->next;
      M = M->next;
    }
  }
  return is_equal;
}
***/

// Manipulation procedures ----------------------------------------------------

void clear(List L)
{
  if (L == NULL)
  {
    printf("List Error: calling clear() on NULL List reference\n");
    exit(1);
  }
  while ( length(L) != 0)
  {
    deleteFront(L);
  }
  L->cursor = NULL;
  L->index = -1; // once length is zero
}

void set(List L, void* x)
{
  if (L == NULL)
  {
    printf("List Error: calling set() on NULL List reference\n");
    exit(1);
  }
  if (L->length == 0 || L->cursor == NULL)
  {
    printf("List Error: calling set() on an empty List\n");
    exit(1);
  }
  L->cursor->data = x; // overwrite cursor element's data
}

void moveFront(List L)
{
  if (L == NULL)
  {
    printf("List Error: calling moveFront() on NULL List reference\n");
    exit(1);
  }
  if (L->length == 0 || L->front == NULL)
  {
    //printf("List Error: calling moveFront() on an empty List\n");
    //exit(1);
    L->cursor = NULL;
    L->index = -1;
  }
  L->cursor = L->front;
  L->index = 0;
}

void moveBack(List L)
{
  if (L == NULL)
  {
    printf("List Error: calling moveBack() on NULL List reference\n");
    exit(1);
  }
  if (L->length == 0 || L->back == NULL)
  {
    printf("List Error: calling moveBack() on an empty List\n");
    exit(1);
  }
  // if cursor at back, move cursor to NULL
  if (L->cursor == L->back)
  {
    L->cursor = NULL;
  }
  else
  {
    L->cursor = L->back;
    L->index = L->length - 1;
  }
}

void movePrev(List L)
{
  if (L == NULL)
  {
    printf("List Error: calling movePrev() on NULL List reference\n");
    exit(1);
  }
  if (L->length == 0 || L->cursor == NULL)
  {
    printf("List Error: calling movePrev() on an empty List\n");
    exit(1);
  }
  if (L->cursor == L->front)
  {
    L->cursor = NULL;
  }
  else
  {
    L->cursor = L->cursor->prev;
    L->index--;
  }
}

void moveNext(List L)
{
  if (L == NULL)
  {
    printf("List Error: calling moveNext() on NULL List reference\n");
    exit(1);
  }
  //if (L->length == 0 || L->cursor == NULL)
  //{
  //  printf("List Error: calling moveNext() on an empty List\n");
  //  exit(1);
  //}
  if (L->cursor == L->back)
  {
    L->cursor = NULL;
  }
  else
  {
    L->cursor = L->cursor->next;
    L->index++;
  }
}

void prepend(List L, void* x)
{
  if (L == NULL)
  {
    printf("List Error: calling prepend() on NULL List reference\n");
    exit(1);
  }
  Node N = newNode(x); // make the node
  if (L->length == 0)
  {
    L->front = L->back = N; // empty list just becomes this node
  }
  else
  {
    L->front->prev = N;
    N->next = L->front;
    L->front = N;
      if(L->cursor != NULL)
        L->index++;
  }
  L->length++;
}

void append(List L, void* x)
{
  if (L == NULL)
  {
    printf("List Error: calling append() on NULL List reference\n");
    exit(1);
  }
  Node N = newNode(x); // make the node
  if (L->length == 0)
  {
    L->front = N;
    L->back = N;  // empty list just becomes this node
  }
  else
  {
    L->back->next = N;
    N->prev = L->back;
    L->back = N;
  }
  L->length++;
}

void insertBefore(List L, void* x)
{
  if (L == NULL)
  {
    printf("List Error: calling insertBefore() on NULL List reference\n");
    exit(1);
  }
  if (L->length == 0 || L->cursor == NULL)
  {
    printf("List Error: calling insertBefore() on an empty List\n");
    exit(1);
  }
  Node N = newNode(x); // make the node
  if (L->cursor == L->front)
  {
    prepend(L, x); // insert before front
  }
  else
  {
    L->cursor->prev->next = N;
    N->prev = L->cursor->prev; // link N to cursor's prev
    N->next = L->cursor;
    L->cursor->prev = N;
    L->index++;
    L->length++;
  }
}

void insertAfter(List L, void* x)
{
  if (L == NULL)
  {
    printf("List Error: calling insertAfter() on NULL List reference\n");
    exit(1);
  }
  if (L->length == 0 || L->cursor == NULL)
  {
    printf("List Error: calling insertAfter() on an empty List\n");
    exit(1);
  }
  Node N = newNode(x); // make the node
  if (L->cursor == L->back)
  {
    append(L, x); // insert after back
  }
  else
  {
    // can't change cursor->next until the new node is inserted
    L->cursor->next->prev = N;
    N->next = L->cursor->next; // link N to cursor's next
    N->prev = L->cursor;
    L->cursor->next = N;
    L->length++;
  }
}


void deleteFront(List L)
{
  if (L == NULL)
  {
    printf("List Error: calling deleteFront() on NULL List reference\n");
    exit(1);
  }
  if (L->length == 0 || L->front == NULL)
  {
    printf("List Error: calling deleteFront() on an empty List\n");
    exit(1);
  }
  
  Node N = NULL;// used to save the old front
  
  if (L->length > 1)
  {
    //printf("hello\n");
    N = L->front;// capture the old front
    L->front = L->front->next;
    freeNode(&N);
    L->front->prev = NULL;
    if (L->index == 0)
      L->cursor = NULL;
    L->index--;
    L->length--;
  }
  else
  {
    //printf("hi\n");
    freeNode(&L->front);
    L->front = L->back = L->cursor = NULL;
    L-> index = -1;
    L-> length = 0;
  }
}

void deleteBack(List L)
{
  if (L == NULL)
  {
    printf("List Error: calling deleteBack() on NULL List reference\n");
    exit(1);
  }
  if (L->length == 0 || L->back == NULL)
  {
    printf("List Error: calling deleteBack() on an empty List\n");
    exit(1);
  }
  
  Node N = NULL;// used to save the old back
  
  if (L->length > 1)
  {
    N = L->back;// capture the old back
    if (L->cursor == N) 
      {
        L->cursor = NULL;
        L->index = -1;
      }
      L->back = L->back->prev;// the one right before the back
      freeNode(&N);
      L->back->next = NULL;
      L->length--;
  }
  else // single node list
  {
    freeNode(&L->front);
    L->front = L-> back = L-> cursor = NULL;
    L->index = -1;
    L->length = 0;
  }  
}

void delete(List L) {
  if (L == NULL) {
    printf("List Error: calling delete() on NULL List reference");
    exit(1);
  }
  if (L->cursor == NULL) {
    printf("List Error: calling delete() on NULL cursor\n");
    exit(1);
  }
  if (L->cursor == L->front) {
    deleteFront(L);
    
  } else if (L->cursor == L->back) {
    deleteBack(L);
  } else {
    // "ignore" cursor in middle
    L->cursor->prev->next = L->cursor->next;
    L->cursor->next->prev = L->cursor->prev;
    free(L->cursor);// delete cursor
    L->cursor = NULL;
    L->length--;
  }
}

void printList(FILE* out, List L)
{
  if (L == NULL)
  {
    printf("List Error: calling printList() on NULL List reference\n");
    exit(1);
  }
  Node N = NULL;
  for (N = L->front; N != NULL; N = N->next)
  {
    fprintf(out, "%f ", *(double*)(N->data));
  }
  printf("\n");
}

/***
List copyList(List L) 
{
  List Copy  = newList();
  Node N = L->front;
  while (N != NULL) {
    append(Copy, N->data);
    N = N->next;
  }
  return Copy;
}
***/
