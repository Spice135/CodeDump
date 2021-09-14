/******************************************************************************/
/*
File:   matrix.c
Author: Akshat Madan
Email:  akshat.madan@digipen.edu
Course: CS225
Description:  
  This file contains the implementation of all the matrix functions.
*/
/******************************************************************************/
#include "matrix.h" /* Function prototypes */
#include <stdlib.h> /* malloc, free */
#include <stdio.h>  /* printf */

/******************************************************************************/
/*
This function prints a matrix.
*/
/******************************************************************************/
void matrix_print( Matrix m, int num_rows, int num_columns ) {
  int i, j; /* Loop variables */
  for ( i=0; i<num_rows; ++i ) { /* For all rows */
    for ( j=0; j<num_columns; ++j ) { /* For all columns */
        printf( "%4i ", m[i][j] ); /* Print matrix */
    }
    printf( "\n" ); /* Newline */
  }
}

/******************************************************************************/
/*
This function allocates a matrix.
*/
/******************************************************************************/
Matrix matrix_create(int num_rows, int num_columns)
{
  int i; /* Loop variable */
  Matrix matrix = (int **)malloc(num_rows * sizeof(int *)); /* Allocate rows */

  for(i = 0; i < num_rows; i++) /* For all rows */
    matrix[i] = (int *)malloc(num_columns * sizeof(int)); /* Allocate columns */

  return matrix; /* Return created matrix */
}

/******************************************************************************/
/*
This function adds 2 matrices.
*/
/******************************************************************************/
void matrix_add( Matrix m1, Matrix m2, Matrix result, int num_rows, int num_columns )
{
  int i, j; /* Loop variable */
  for(i = 0; i < num_rows; i++) /* For all rows */
    for(j = 0; j < num_columns; j++) /* For all columns */
      result[i][j] = m1[i][j] + m2[i][j]; /* Add matrices and store in resultant matrix */
}

/******************************************************************************/
/*
This function transposes a matrix.
*/
/******************************************************************************/
Matrix matrix_transpose( Matrix m, int num_rows, int num_columns )
{
  int i, j; /* Loop variables */
  Matrix transpose = matrix_create(num_columns, num_rows); /* Allocate matrix with rows and columns flipped to store transpose */

  for(i = 0; i < num_rows; i++) /* For all rows */
    for(j = 0; j < num_columns; j++) /* For all columns */
      transpose[j][i] = m[i][j]; /* Store transpose in newly allocated matrix */

  return transpose; /* Return transpose matrix */
}

/******************************************************************************/
/*
This function deletes a matrix.
*/
/******************************************************************************/
void matrix_delete( Matrix m, int num_rows )
{
  int i; /* Loop variable */
  for(i = 0; i < num_rows; i++) /* For all rows */
    free(m[i]); /* Delete column allocation */
  free(m); /* Delete rows allocation */
}

/******************************************************************************/
/*
This function deletes a row from a matrix.
*/
/******************************************************************************/
void matrix_delete_row( Matrix m, int r, int num_rows )
{
  int i; /* Loop variable */
  free(m[r]); /* Free the row */
  for(i = r; i < num_rows - 1; i++) /* Do for all rows after r */
    m[i] = m[i + 1]; /* Make next matrix row, this one */
}

/******************************************************************************/
/*
This function deletes a column from a matrix.
*/
/******************************************************************************/
void matrix_delete_column( Matrix m, int c, int num_rows, int num_columns )
{
  int i, j; /* Loop variables */
  for(i = c; i < num_columns - 1; i++) /* For all columns */
    for(j = 0; j < num_rows; j++) /* For all rows */
      m[j][i] = m[j][i + 1]; /* Move all columns up one space */
}