/******************************************************************************/
/*
File:   uucode.cpp
Author: Akshat Madan
Email:  akshat.madan@digipen.edu
Course: CS225
Description:  
  This file contains the implementation of uuencode and uudecode.
*/
/******************************************************************************/
#include "uucode.h" // Function prototypes
#include <iostream> // cout
#include <stdio.h>  // FILE, fread, fputc, fgets
#include <string.h> // strlen
#include <fstream>  // fstream

/******************************************************************************/
/*!
  \brief
    Encodes a file using the uuencode algorithm
  
  \param InputFilename
    File to be encoded
  
  \param RemoteFilename
    Output file name (to be placed in line 1 of encoded file)
    
  \return
    Returns any errors with opening files
*/
/******************************************************************************/
int uuencode(const char *InputFilename, const char *RemoteFilename)
{
  FILE* inputFile = fopen(InputFilename, "rb"); // File to be encoded
  char buffer[3] = {0}; // Buffer that reads in characters
  char encodedBuffer[60] = {0}; // Buffer to hold encoded characters
  int charCount = 0; // Number of characters read (for line's first char)
  int numPasses = 0; // Number of passes made through the file

  std::cout << "begin 644 " << RemoteFilename << std::endl; // Print begin and filename

  if(!inputFile)
    return -1; // Return error in opening file

  while(!feof(inputFile)) // While not at the end of file
  {
    size_t numRead = fread(buffer, 1, 3, inputFile); // Read 3 characters

    if(numRead == 2) // If only 2 characters were read
    {
      buffer[2] = 0; // Pad third character as 0s
    }
    else if(numRead == 1) // If only one character was read
    {
      buffer[1] = 0; // Pad out last two characters
      buffer[2] = 0; // Pad out last two characters
    }
    else if(numRead == 0) // If no characters were read
      break; // Break from while loop (as we are at the end of file)

    encodedBuffer[charCount] = (((unsigned char)buffer[0] >> 2) & 0x3F) + 32; // Get first 6 bits
    encodedBuffer[charCount + 1] = ((((unsigned char)buffer[0] << 4) | ((unsigned char)buffer[1] >> 4)) & 0x3F) + 32; // Get next 6 bits
    encodedBuffer[charCount + 2] = ((((unsigned char)buffer[1] << 2) | ((unsigned char)buffer[2] >> 6)) & 0x3F) + 32; // Get next 6 bits
    encodedBuffer[charCount + 3] = ((unsigned char)buffer[2] & 0x3F) + 32; // Get last 6 bits; Hex for 00111111

    charCount += numRead + 1; // Increment number of characters read in
    numPasses++; // Increment number of passes made

    if(charCount > 59) // If character count is greater than 59
    {
      std::cout << (char)(charCount + 32 - numPasses); // Print 'M' signifying 45 chars in the line

      for(int i = 0; i < charCount; i++) // Loop through encodedBuffer
      {
        if(encodedBuffer[i] == 32)
          std::cout << "`"; // If its space, print backquote
        else
          std::cout << encodedBuffer[i]; // Otherwise, print buffer

        encodedBuffer[i] = 0; // Empty buffer
      }

      std::cout << std::endl; // End the line
      charCount = 0; // Set character count to 0
      numPasses = 0; // Set number of passes to 0
    }
  }

  if(charCount != 0) // If at the end of file, and encoded buffer has some characters
  {
    std::cout << (char)(charCount + 32 - numPasses); // Print number of characters in the line

    for(int i = 0; i <= charCount + 1; i++) // Loop through encodedBuffer
    {
      if(encodedBuffer[i] == 32)
        std::cout << "`"; // If its space, print backquote
      else if (encodedBuffer[i] == 0)
        break; // If its NULL, break from loop
      else
        std::cout << encodedBuffer[i]; // Otherwise, print buffer
    }
  }

  std::cout << std::endl << "`" << std::endl << "end" << std::endl; // Print final backquote and end and newline
  fclose(inputFile); // Close the input file
  return 0; // Return all clear!
}

/******************************************************************************/
/*!
  \brief
    Decodes a file using the uuencode algorithm
  
  \param InputFilename
    File to be decoded
    
  \return
    Returns any errors with opening files
*/
/******************************************************************************/
int uudecode(const char *InputFilename)
{
  std::fstream inStream; // Input file stream
  std::fstream outStream; // File that stores decoded data
  char fileName[256] = {0}; // Name of output file

  inStream.open(InputFilename, std::ios::in); // Open input file
  if(!inStream.is_open())
    return -1; // Return error in opening file

  inStream.get(fileName, 11); // Read in "begin 644 "
  inStream.get(fileName, 256); // Read in actual filename

  for(int i = 0; i < 256; i++) // Loop through fileName buffer
  {
    if(*(fileName + i) == '\n') // If the char is newline
    {
      if(*(fileName + i - 1) == 0x0D) // Check if previous character was CR (Windows)
        *(fileName + i - 1) = 0; // Set it to NULL

      *(fileName + i) = 0; // Strip trailing newline character
      break; // Break from for loop
    }
  }

  outStream.open(fileName, std::ios::binary | std::ios::out); // Open output file
  if(!outStream.is_open())
    return -1; // Return error in opening file

  char buffer[61]; // Buffer to store whats read in
  int charWritten = 0; // Number of character's written
  
  while(!inStream.eof()) // If not at the end of file
  {
    inStream.getline(buffer, 62); // Get the line
    int length = inStream.gcount(); // Get line count
    int charCount = *buffer - 32; // Get number of characters on the line
    
    if(*buffer == '`')
        break; // If its a single backquote, end of file
    
    for(int i = 1; i <= length - 1; i += 4) // Loop until the end of line
    {
      unsigned char decodedBuffer[3] = {0}; // Buffer to store decoded characters
      
      for(int j = 0; j <= 3; j++)
        if(*(buffer + i + j) == '`') // If the character is a backquote
          *(buffer + i + j) = ' '; // Change it to space
      
      *decodedBuffer = (((*(buffer + i) - 32) << 2) & 0xFC) | (((*(buffer + i + 1) - 32) >> 4) & 0x03); // First character
      *(decodedBuffer + 1) = (((*(buffer + i + 1) - 32) << 4) & 0xF0) | (((*(buffer + i + 2) - 32) >> 2) & 0x0F); // Second character
      *(decodedBuffer + 2) = (((*(buffer + i + 2) - 32) << 6) & 0xC0) | ((*(buffer + i + 3) - 32) & 0x3F); // Third character
      
      for(int j = 0; j <= 2; j++) // Loop for 3 decoded characters
        if(charWritten < charCount) // If there are less characters than the number of characters
        {
          outStream << *(decodedBuffer + j); // Keep writing to file
          charWritten++; // Increment number of characters written
        }
    }

    charWritten = 0; // Set number of characters written to 0
  }

  return 0; // Return everything is awesome!
}