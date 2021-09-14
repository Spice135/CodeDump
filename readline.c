/******************************************************************************/
/*
File:   readline.c
Author: Akshat Madan
Email:  akshat.madan@digipen.edu
Course: CS225
Description:  
  This file contains the implementation of the readline function and also
  implements doublebuffer as a helper function.
*/
/******************************************************************************/
#include "readline.h" /* Function prototype, stdio.h */
#include <stdlib.h> /* File */
#include <string.h> /* strlen */

char * doublebuffer(char * buffer, int * size); /* Function prototype */

/******************************************************************************/
/*
This function takes in a file pointer and reads in strings from the file until
it reaches the end of the file or a newline character
*/
/******************************************************************************/
char * readline(FILE * openFile)
{
  int size = 1; /* Set initial size to 1 */
  int readSize = 2; /* Set initial read size to 2 (as fgets reads num - 1) */
  int iteration = 0; /* Number of iterations */
  int flag = 0; /* Flag for if a newline is found */
  char * buffer = (char *)calloc(1, size + 1); /* Allocate size++ (terminating char) */

  while(!feof(openFile) && flag == 0) /* While !feof or flag for newline = 0 */
  {
    if((int)strlen(buffer) == size) /* If the buffer is too small */
    {
      int prevSize = size; /* Temporarily store previous size */
      buffer = doublebuffer(buffer, &size); /* Double the size */
      readSize = size - prevSize + 1; /* Increase the read size to fill buffer */
    }

    if(fgets(buffer + iteration, readSize, openFile) != NULL) /* read characters */
    {
      iteration = strlen(buffer); /* Set offset to string length */
      if(buffer[strlen(buffer) - 1] == '\n') /* Last copied character */
        flag = 1; /* Set flag to true */
    }
    else
      flag = 1; /* Set flag to true */
  }

  if(buffer[strlen(buffer) - 1] == '\n') /* If last character is newline */
    buffer[strlen(buffer) - 1] = '\0'; /* Strip trailing newline character */
  return buffer; /* Return the buffer */
}

/******************************************************************************/
/*
This helper function takes in a character pointer to the buffer and a integer
pointer to the size of that buffer. It doubles the size of the buffer and
returns a new buffer with the information copied in.
*/
/******************************************************************************/
char * doublebuffer(char * buffer, int * size)
{
  int newSize = (*size) * 2; /* Set the new size to be twice the old size */
  char * newBuffer = (char *)calloc(1, newSize + 1); /* Allocate a new buffer */
  int i = 0; /* Counter for for loop */

  for(i = 0; i <= *size; i++)
    newBuffer[i] = buffer[i]; /* Copy old buffer into new buffer */

  free(buffer); /* Free memory of old buffer */
  *size = newSize; /* Increase size to match new buffer */

  return newBuffer; /* Return the new buffer */
}