/******************************************************************************/
/*
File:   splitter.c
Author: Akshat Madan
Email:  akshat.madan@digipen.edu
Course: CS225
Description:  
  This file contains the implementation of the SplitFile and JoinFile function.
*/
/******************************************************************************/
#include <stdio.h>     /* printf, sprintf, file */
#include <stdlib.h>    /* calloc */
#include "splitter.h"  /* SplitFile, JoinFiles */

#define minSize 4096   /* Define minimum buffer size to 4K */

/******************************************************************************/
/*!
  \brief
    Splits file into multiple smaller fies or chunks.
  
  \param filename
    File to be split
  
  \param output
    Naming convention for output files
  
  \param size
    Size of chunks
    
  \return
    Returns any errors with opening files, or memory allocation
*/
/******************************************************************************/
int SplitFile(char const *  filename,  char const * output, size_t size)
{
  char current_file_name[256]; /* Character array for output file names */
  int file_count = 1; /* Number of output files created */
  char* buffer; /* Create a buffer */
  FILE * outputFile; /* File to store output */
  FILE * readFile = fopen(filename, "rb"); /* File to read from */

  if(size > minSize)
    buffer = (char *)malloc(size + 1); /* Allocate buffer of size + 1 */
  else
    buffer = (char *)malloc(minSize + 1); /* Allocate buffer of minimum size */

  if(!readFile)
  {
    free(buffer); /* Free allocated buffer */
    return E_BAD_SOURCE; /* Fopen source failed error code */
  }
  if(!buffer)
  {
    fclose(readFile); /* Close opened file */
    return E_NO_MEMORY; /* Malloc failed error code */
  }

  while(!feof(readFile)) /* Do while not at the end of file */
  {
    /* print in to a string. The format is string(%s) 
     * and a 4-digit integer padded with 0's on the left, i.e.
     * 1 is printed as 0001, 123 as 0123, note that 12345 is printed as 12345 */
    sprintf(current_file_name,"%s%04lu",output,(long unsigned int)file_count++);

    fgets(buffer, (int)size + 1, readFile); /* Read upto size into buffer */
    if(!feof(readFile)) /* If not at the end of file (prevents writing last buffer to 2 files) */
    {
      /* Open the output file for writing. If file does not exist, create one */
      outputFile = fopen(current_file_name, "wb");

      if(!outputFile)
      {
        fclose(readFile); /* Close opened file */
        free(buffer); /* Free allocated buffer */
        return E_BAD_DESTINATION; /* Fopen destination failed error code */
      }

      buffer[size + 1] = '\n'; /* Write newline at the end of file */
      fputs(buffer, outputFile); /* Write the buffer to the file */
      fclose(outputFile); /* Close the output file */
    }
  }

  fclose(readFile); /* Close opened file */
  free(buffer); /* Free the buffer */
  return 0; /* Return that the function ran without errors */
}

/******************************************************************************/
/*!
  \brief
    Joines chunks or smaller files into a single larger file.
  
  \param filenames
    List of filenames to be joined
  
  \param num_files
    Number of smaller files that exist
  
  \param output
    Name of output file
    
  \return
    Returns any errors with opening files, or memory allocation
*/
/******************************************************************************/
int JoinFiles(char** filenames, int num_files, char const * output) 
{
  char *buffer = (char *)malloc(minSize + 1); /* Allocate a buffer of minimum size */
  FILE *outFile = fopen(output, "wb"); /* Open file for writing */

  if(!outFile)
  {
    free(buffer); /* Free buffer */
    return E_BAD_DESTINATION; /* Fopen destination failed error code */
  }
  if(!buffer)
  {
    fclose(outFile); /* Close opened file */
    return E_NO_MEMORY; /* Malloc failed error code */
  }

  while (*filenames) /* While not at the end of list of filenames */
  {
    FILE *openFile = fopen(*filenames, "rb"); /* Open file for read */

    if(!openFile)
    {
      free(buffer); /* Free allocated buffer */
      fclose(outFile); /* Close opened file */
      return E_BAD_SOURCE; /* Fopen source failed error code */
    }

    while(fgets(buffer, minSize + 1, openFile)) /* Read upto minimum size into buffer */
      fputs(buffer, outFile); /* Write buffer into output file */
    
    fclose(openFile); /* Close the open file */

    filenames++; /* Do for next filename */
    num_files--; /* Reduce number of files */
  }

  fclose(outFile); /* Close output file */
  free(buffer); /* Free the buffer */
  return 0; /* Return with no errors */
}
