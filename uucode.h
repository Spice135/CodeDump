/******************************************************************************/
/*
File:   uucode.h
Author: Akshat Madan
Email:  akshat.madan@digipen.edu
Course: CS225
Description:  
  This file contains the implementation of uuencode and uudecode.
*/
/******************************************************************************/
/*---------------------------------------------------------------------------*/
#ifndef UUCODE_H
#define UUCODE_H
/*---------------------------------------------------------------------------*/

int uuencode(const char *InputFilename, const char *RemoteFilename); // Prototype for encode
int uudecode(const char *InputFilename); // Prototype for decode

#endif
