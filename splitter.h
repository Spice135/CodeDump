/******************************************************************************/
/*
File:   splitter.h
Author: Akshat Madan
Email:  akshat.madan@digipen.edu
Course: CS225
Description:  
  This file contains the implementation of the SplitFile and JoinFile function.
*/
/******************************************************************************/
#ifndef SPLITTER_H
#define SPLITTER_H
#include <stddef.h> /* size_t definition */

/* Return types for SplitFile and JoinFiles functions */
enum 
{E_BAD_SOURCE=1, E_BAD_DESTINATION, E_NO_MEMORY, E_NO_ACTION, E_SMALL_SIZE};

#ifdef __cplusplus
extern "C" {  //only visible by C++ compiler
#endif

/* Function for splitting files into smaller chunks */
int SplitFile(char const *  filename,  char const * output, size_t size);

/* Function for joining files from chunks into a single, bigger file */
int JoinFiles(char** filenames, int num_files, char const * output);

#ifdef __cplusplus
} //only visible by C++ compiler
#endif

#endif 

