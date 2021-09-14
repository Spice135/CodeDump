#include <stdlib.h>
#include <stdio.h>
#include "spvector.h"

/* definition of Node is hidden from the client (driver) so that client CANNOT
 * use INTERNALS of the Node.  this is poor-man encapsulations - hiding
 * implementation details.  The main reason for encapsulations -- if I ever
 * decide to change Node struct no client code will break. Example: I decide to
 * change "left" to "Left", all I do is change map.h and map.c, no client code
 * is effected, since clients were FORCED to use getleft(...) function, rather
 * than field name "left".  Also see typedef in map.h
 */
struct ElementNode 
{
  int    data;
  int    pos;
  struct ElementNode* next;
};

struct RowNode 
{
  int pos;
  struct ElementNode* elements;
  struct RowNode* next;
};

typedef struct ElementNode ElementNode;
typedef struct RowNode RowNode;


/*print functions*/
void printf_elements(ConstElementNode_handle p_e, char *fmt, int dim) 
{
	int i, last_pos = -1;
	while (p_e) 
  {
		for (i = last_pos + 1; i < p_e->pos; ++i) 
    { 
      printf(fmt,0); 
    }

		printf(fmt,p_e->data);
		last_pos=p_e->pos;
		p_e = p_e->next;
	}

	for (i=last_pos+1;i<dim;++i) 
  { 
    printf(fmt,0);
  }
}

void print_elements(ConstElementNode_handle p_e) 
{
	while (p_e) 
  {
		printf("%i at pos %i, ",p_e->data,p_e->pos);
		p_e = p_e->next;
	}
}

void print_rows(ConstRowNode_handle p_r) 
{
	while (p_r) 
  {
		printf("row %i: ", p_r->pos);
		print_elements(p_r->elements);
		printf("\n");
		p_r = p_r->next;
	}
}

void printf_rows(ConstRowNode_handle p_r, char *fmt, int dim) 
{
	int i,j,last_pos=-1;
	while (p_r) 
  {
		for (i = last_pos + 1; i < p_r->pos; ++i) 
    { /* output row of 0's */
			for (j = 0; j < dim; ++j) 
        printf(fmt,0); 
			printf("\n");
		}

		printf_elements(p_r->elements,fmt,dim);
		printf("\n");
		last_pos = p_r->pos;
		p_r = p_r->next;
	}

	for (i = last_pos + 1; i < dim; ++i) 
  { /* output row of 0's */
		for (j = 0; j < dim; ++j) 
      printf(fmt,0); 
		printf("\n");
	}
}


int  determinant(ConstRowNode_handle p_r,int dim) 
{
	/* this is a meaningless code to get rid of "unsed argument" warnings in 
	 * Borland an MS */
	int result = 1;

	if (dim == 0)
    ++result;

	if (p_r)
    --result;

	return result;
}

/* insert an element into a list 
 * list is ordered using pos
 * if position pos is already occupied, the value of the node
 * should be updated with val
 * if val=0, then the element should be deleted
 * return 0 if operation is succesful 
 *        1 if malloc failed */
int insert_element(ElementNode_handle * pp_e, int pos, int val)
{
  if(!val)
  {
    delete_element(pp_e, pos);
    return 0;
  }
  else
  {
    ElementNode_handle newNode = calloc(1, sizeof(ElementNode));
    if(!newNode)
      return 1;
    newNode->pos = pos;
    newNode->data = val;
    while (*pp_e != NULL && (*pp_e)->pos < newNode->pos)
      pp_e = &((*pp_e)->next);
    newNode->next = *pp_e;
    *pp_e = newNode;
  }

  return 0;
}

/* 
 * delete an element at position pos if it exists */
void delete_element(ElementNode_handle * pp_e, int pos)
{
  ElementNode_handle p_Last = *pp_e;
  if(!(*pp_e))
    return;

  while (*pp_e != NULL && (*pp_e)->pos < pos)
  {
    p_Last = *pp_e;
    pp_e = &((*pp_e)->next);
  }

  if((*pp_e)->pos == pos)
  {
    p_Last->next = (*pp_e)->next;
    free(*pp_e);
  }
}

/*
 * get the value at the given position,
 * p_e is the head pointer 
 */
int get(ConstElementNode_handle p_e, int pos)
{
  while(p_e)
  {
    if(p_e->pos == pos)
      return p_e->data;
    else
      p_e = p_e->next;
  }

  return 0;
}

/* 
 * scalar product of 2 lists.
 * */
int scalar_product(ConstElementNode_handle p_e1, ConstElementNode_handle p_e2)
{
  int sum = 0;
  while(p_e1 && p_e2)
  {
    while(p_e1->pos < p_e2->pos)
    {
      if(p_e1->next)
        p_e1 = p_e1->next;
      else
        break;
    }

    while(p_e2->pos < p_e1->pos)
    {
      if(p_e2->next)
        p_e2 = p_e2->next;
      else
        break;
    }
    
    if(p_e1->pos == p_e2->pos)
      sum += p_e1->data * p_e2->data;
    p_e1 = p_e1->next;
    p_e2 = p_e2->next;
  }

  return sum;
}

/* 
 * adds 2 lists as vectors, returns a new list */
ElementNode_handle add(ConstElementNode_handle p_e1, ConstElementNode_handle p_e2)
{
  ElementNode_handle p_return = 0;

  if(p_e1 && !p_e2)
  {
    p_return = (ElementNode_handle)p_e1;
    return p_return;
  }
  else if(!p_e1 && p_e2)
  {
    p_return = (ElementNode_handle)p_e2;
    return p_return;
  }
  else if(!p_e1 && !p_e2)
    return p_return;

  while(p_e1 || p_e2)
  {
    int sum = 0;

    if(!p_e1 && p_e2)
    {
      insert_element(&p_return, p_e2->pos, p_e2->data);
      break;
    }
    if(p_e1 && !p_e2)
    {
      insert_element(&p_return, p_e1->pos, p_e1->data);
      break;
    }

    if(p_e1->pos == p_e2->pos)
    {
      sum = p_e1->data + p_e2->data;
      insert_element(&p_return, p_e1->pos, sum);
      p_e2 = p_e2->next;
      p_e1 = p_e1->next;
    }
    else if(p_e1->pos < p_e2->pos && (p_e1->next)->pos >= p_e2->pos)
    {
      insert_element(&p_return, p_e1->pos, p_e1->data);
      p_e1 = p_e1->next;
    }
    else if(p_e2->pos < p_e1->pos && (p_e2->next)->pos >= p_e1->pos)
    {
      insert_element(&p_return, p_e2->pos, p_e2->data);
      p_e2 = p_e2->next;
    } 
    
  }

  return p_return;
}

/* 
 * deallocate a list */
void free_elements(ElementNode_handle p_e)
{
  ElementNode_handle nextNode = 0;
  while(p_e)
  {
    if(p_e->next)
      nextNode = p_e->next;
    free(p_e);
    p_e = nextNode;
  }
}











/*===============* 
 * row functions * 
 *===============*/

/* 
 * inserts a list of elements at position pos.
 * If a row at position pos already exists, do not insert, return 2
 * If the list p_e is empty, do nothing (don't insert a NULL pointer into p_r)
 * returns 1 if malloc fails
 * returns 2 if position pos already exists
 * returns 0 otherwise */
int insert_row(RowNode_handle * pp_r, int pos, ElementNode_handle p_e)
{
  RowNode_handle * pp_rLast;

  while((*pp_r)->pos > pos)
  {
    (*pp_rLast) = (*pp_r);
    (*pp_r) = (*pp_r)->next;
  }

  if((*pp_r)->pos == pos && (*pp_r))
    return 2;
  else
  {
    RowNode_handle * pp_rNext = 0;
    if(pp_r)
      pp_rNext = pp_r;
    pp_r = calloc(1, sizeof(RowNode_handle));
    if(!pp_r)
      return 1;
    (*pp_rLast)->next = (*pp_r);
    (*pp_r)->elements = p_e;
    (*pp_r)->next = *pp_rNext;
  }

  return 0;
}

/* inserts a new element at pos_col 
 * in the row corresponding to position pos_row
 * (pos_row'th row, pos_col'th column) 

 * if position pos is already occupied, the value of the node
 * should be updated with val
 * if val=0, then the element should be deleted
 * return 0 if operation is succesful 
 *        1 if malloc failed

 * uses the same rules as insert_element */
int insert_element2(RowNode_handle * pp_r, int row, int column, int val)
{
  while(*pp_r)
  {
    if((*pp_r)->pos == row)
    {
      int inElement = insert_element(&(*pp_r)->elements, column, val);
      return inElement;
    }
    else
      (*pp_r) = (*pp_r)->next;
  }

  return 0;
}

/* returns a pointer to a row at position pos if it exists, 
 * NULL otherwise */
RowNode_handle find_row(RowNode_handle * pp_r, int val)
{
  while((*pp_r))
  {
    if((*pp_r)->pos == val)
      return (*pp_r);
    else
      (*pp_r) = (*pp_r)->next;
  }

  return 0;
}

/* transposes a list of rows:
 * it's the matrix transposition if you view rows as a matrix
 * anyways - if an element in the original RowNode list 
 * is on row i at position j, then in the transposed
 * it is in the row j at position i. 
 * Notice that it's possible to get empty rows in the transposed
 * matrix (for example the following matrix
 * [ 1 0 4 ]     [ 1 2 3 ]
 * [ 2 0 5 ] ->  [ 0 0 0 ]
 * [ 3 0 6 ]     [ 4 5 6 ]
 * for full credit the transposed matrix should contain only 2 
 * rows (positions 0 and 2) 
 * Returns a new list of rows. Input list p_r should not be modified.*/
RowNode_handle transpose(ConstRowNode_handle p_r)
{
  RowNode_handle newNode = p_r->next;
  return newNode;
}

/* multiply two lists of RowNodes as matricies.
 * Algorithm: 
 * 1) transpose the second list.
 * 2) Now [i,j]'th element of the product matrix
 * is the scalar product of i'th row of the first
 * and the j'th row of the transposed. 
 * Returns a new list of rows. Input lists p_r1, p_r2 should not be modified. */
RowNode_handle mult(ConstRowNode_handle p_r1, ConstRowNode_handle p_r2)
{
  RowNode_handle result = transpose(p_r2);
  result = transpose(p_r1);
  /* result = scalar_product(p_r1->elements, p_r2->elements); */
  return result;
}

/* 
 * deallocate a list of RowNodes */
void free_rows(RowNode_handle p_r)
{
  RowNode_handle p_rNext = p_r;
  while(p_r)
  {
    p_rNext = p_r->next;
    free(p_r);
    p_r = p_rNext;
  }
}