//-----------------------------------------------------------------------------
// Matrix.c
// Matrix ADT
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include "List.h"
#include "Matrix.h"

// private EntryObj type
typedef struct EntryObj {
  int column;
  double data;
}
EntryObj;

// private Entry type
typedef EntryObj* Entry;

// public MatrixObj type
typedef struct MatrixObj{
  int size;
  int nnz;
  List* data;
}
MatrixObj;

//constructors and destructors
Entry newEntry(int n, double x){
  Entry E = malloc(sizeof(EntryObj));
  E->column = n;
  E->data = x;
  return(E);
}

void freeEntry(Entry *pE){
if( pE!=NULL && *pE!=NULL )
  {
    free(*pE);
    *pE = NULL;
  }
}

Matrix newMatrix(int n){
  Matrix M = malloc(sizeof(MatrixObj));
  M->data = malloc(sizeof(List) * n);
  M->size = n;
  M->nnz = 0;
  for (int i = 0; i < n; i++){
    M->data[i] = newList();
  }
  return M;
}

void freeMatrix(Matrix* pM){
  Matrix M = *pM;
  for (int i = 0; i < (*pM)->size; i++){
      freeList(&(M->data[i]));
  }
    free(M);
    *pM = NULL;
  
}

// Access functions
int size(Matrix M){
  if (M == NULL)
  {
    printf("Matrix Error: calling size() on NULL Matrix reference\n");
    exit(1);
  }
  return M->size;
}

int NNZ(Matrix M){
if( M == NULL )
	{
   	printf("Matrix Error: calling NNZ() on NULL Matrix reference\n");
   	exit(1);
	}
  return M->nnz;
}

int equals(Matrix A, Matrix B){ 
	if( A==NULL || B==NULL )
	{
   	printf("Matrix Error: calling equals() on NULL Matrix reference\n");
   	exit(1);
	}
  if (A == B){
    return 1;
  }
  Entry E;
  Entry F;
  int i;
  if(A->size != B->size){
	  return 0;
  }
	for(i=0; i < A->size; i++)
	{
    List L = A->data[i];
    List C = B->data[i];
		if(length(L) != length(C)){
			return 0;
    }
		moveFront(L);
		moveFront(C);
		while(index(L) > -1)
		{
      E = get(L);
      F = get(C);
			if(E->column != F->column){
				return 0;
      }
			if(E->data != F->data){
				return 0;
      }
        moveNext(L);
        moveNext(C);
		}
	}
	return 1;
}

// Manipulation procedures
void makeZero(Matrix M)
{
  int i;
  List L;
  Entry E;
	if( M==NULL )
   {
      printf("Matrix Error: calling makeZero() on NULL Matrix reference\n");
      exit(1);
   }
  M->nnz = 0;
  for(i = 0; i < M->size; i++)
  {
      L = M->data[i];
      moveFront(L);
      while(index(L) > -1)
      {
        E = get(L);
        //deleteFront(L);
        freeEntry(&E);
        moveNext(L);
      }
      clear(L);
  }
}

void changeEntry(Matrix M, int i, int j, double x){
  Entry E;
  if ((i < 1 || M->size < i) || (1 > j || j > M->size)){ 
    printf("Matrix Error: calling changeEntry() when i and j out of bounds\n");
    exit(1);
  }
  if( M==NULL )
	{
   	printf("Matrix Error: calling changeEntry() on NULL Matrix reference\n");
   	exit(1);
	}

    List L = M->data[i - 1]; 
    // search to see if column j is in list
    moveFront(L); // if length L > 0 , call move front
    while(index(L) >= 0){
      E = get(L); 
      if (E->column == j){ // finding column j in list
        if (x != 0) {
        E->data = x; // overwriting if x and Mij are nonzero
        }
        else{
          delete(L); // delete if x == 0 
          M->nnz--;
        }
        return;
      }
      moveNext(L);
    }
    if (x == 0){ // x and Mij are zero, do nothing
      return;
    }
    // adding nonzero x to L, Mij == 0
    Entry N = newEntry(j,x);
    if (length(L) > 0) {
      moveFront(L);
      while(index(L) >= 0) { 
        // insert before 1st column > j, maintain list order
        E = get(L);
        if (E->column > j) {
          M->nnz++;
          insertBefore(L,N);
          return;
        }
        moveNext(L);
      }
    }
    // add x to empty list, append if j is largest column in L
    append(L,N);
    M->nnz++;
    return;
}


// Matrix Arithmetic operations
Matrix copy(Matrix A){
	
  if( A == NULL )
	{
   	printf("Matrix Error: calling copy() on NULL Matrix reference\n");
   	exit(1);
  }
  Matrix M;
  List L;
  List C;

  M = newMatrix(A->size);
   for(int i = 0; i < A->size; i++)
   {
      C = M->data[i];
      L = A->data[i];
      moveFront(L);
      while(index(L) > -1)
      {
        Entry E = get(L);
         append(C, newEntry(E->column, E->data));
         moveNext(L);
      }
   }
   M->nnz = A->nnz;
	return M;
}

Matrix transpose(Matrix A){
  if( A==NULL )
	{
   	printf("Matrix Error: calling transpose() on NULL Matrix reference\n");
   	exit(1);
	}
  List L;
  Entry E;
  Matrix M;
  int i;
  M = newMatrix(A->size);
  for (i = 0; i < A->size; i++){
    L = A->data[i];
    moveFront(L);
    while(index(L) > -1){
      E = get(L);
      moveNext(L);
      changeEntry(M, E->column, i + 1, E->data);
    }
  }
  return M;
}



Matrix scalarMult(double x, Matrix A){
	if( A == NULL )
	{
   	printf("Matrix Error: calling scalarMult() on NULL Matrix reference\n");
   	exit(1);
	}
  int i;
  Matrix M;
  List L;
  M = copy(A);
   if(x != 0)
   {
      for(i=0; i < M->size; i++)
      {
         L = A->data[i];
         moveFront(L);
         while(index(L) > -1)
        {
          Entry E = get(L);
          changeEntry(M, i + 1, E->column, E->data * x);
          moveNext(L);
        }
      }
   }
	return M;
}


List sumList(List A, List B){
  moveFront(A);
  moveFront(B);
  Entry a;
  Entry b;
  List L = newList();

  while (index(A) > -1 && index(B) > -1){
    // 3 cases 
    // a = b
    a = get(A);
    b = get(B);
    if (a->column == b->column) {
      if (a->data + b->data != 0){
        append(L, newEntry(a->column, a->data + b->data));
      }
      moveNext(A);
      moveNext(B);
    }
    if (a->column < b->column) {
      append(L, newEntry(a->column, a->data));
      moveNext(A);
    }
    if (b->column < a->column) {
      append(L, newEntry(b->column, b->data));
      moveNext(B);
    } 
  }
  while (index(A) > -1){
    a = get(A);
    append(L, newEntry(a->column, a->data));
    moveNext(A);
  }
  while (index(B) > -1){
    b = get(B);
    append(L, newEntry(b->column, b->data));
    moveNext(B);
  }
  return L;
}

Matrix sum(Matrix A, Matrix B){
  if( A==NULL || B==NULL )
	{
   	printf("Matrix Error: calling sum() on NULL Matrix reference\n");
   	exit(1);
	}
  if(A->size != B->size)
  {
    printf("Matrix Error: calling sum() on matrices of different sizes\n");
    exit(1);
  }
  if(A == B)
  {
    return(scalarMult(2, A));
  }
  Matrix C = newMatrix(A->size);
  for (int i = 0; i < A->size; i++){
    C->data[i] = sumList(A->data[i], B->data[i]);
    C->nnz+=length(C->data[i]);
  }
  return C;
}



Matrix diff(Matrix A, Matrix B){
	if( A==NULL || B==NULL )
	{
   	printf("Matrix Error: calling diff() on NULL Matrix reference\n");
   	exit(1);
	}
  if(A->size != B->size)
  {
    printf("Matrix Error: calling diff() on matrices of different sizes\n");
    exit(1);
  }
  Matrix C, D;
  C = scalarMult(-1, B);
  D = sum(A, C);
  freeMatrix(&C);
  return D;
}

  

double vectorDot(List A, List B){ 
  //edge case : list different sizes
  //edge case ; if either list is empty, return copy of other list
  // return 0 if list is empty
  if (length(A) == 0 || length(B) == 0){
    return 0;
  }
  moveFront(A);
  moveFront(B);
  Entry a;
  Entry b;
  double value = 0.0;

  while (index(A) > -1 && index(B) > -1){
    a = get(A);
    b = get(B);
    if (a->column == b->column){
      value += a->data * b->data;
      moveNext(A);
      moveNext(B);
    }
    if (a->column < b->column){
      value += 0;
      moveNext(A);
    }
    if (b->column < a->column){
      value += 0;
      moveNext(B);
    }
  }
  return value;
}

Matrix product(Matrix A, Matrix B){ 
  if( A==NULL || B==NULL )
	{
   	printf("Matrix Error: calling product() on NULL Matrix reference\n");
   	exit(1);
	}
  if(A->size != B->size)
   {
      printf("Matrix Error: calling product() on matrices of different sizes\n");
      exit(1);
   }
   if(A == B)
  {
  B = copy(A);
  }
  Matrix M;
  M = newMatrix(A->size);
  Matrix Bt;
  int i, j;
  double dot;
  Bt = transpose(B);
  for (i = 0; i < A->size; i++){
    for (j = 0; j < A->size; j++){
      dot = vectorDot(A->data[i], Bt->data[j]);
      if (dot != 0){
        changeEntry(M, i + 1, j + 1, dot);
      }
    }
  }
  freeMatrix(&Bt);
  return M;
}


void printMatrix(FILE* out, Matrix M){
//access list in rows and print data pairs
int i;
List L;
Entry E;
	if( M==NULL ) {
   	printf("Matrix Error: calling printMatrix() on NULL Matrix reference\n");
   	exit(1);
	}
  for(i=0; i < M->size; i++)
   {
    if (length(M->data[i]) > 0)
      {
        fprintf(out, "%d: ", i + 1);
        L = M->data[i];
         moveFront(L);
         while (index(L) > -1) {
          E = get(L);
          fprintf(out, " (%d, %.1f) " , E->column, E->data);
          moveNext(L);
        }
      fprintf (out, "\n");
      }
    }
}
