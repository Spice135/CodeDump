// File Name: Wave.cpp
// Author: Akshat Madan
// Course: CS245
// Project: Assignment 03
// Date: 01/26/2022

#include "AudioData.h"
#include <math.h> // powf, fabs
#include <fstream> // File operations

// Creates audio data by reading from file named fname
AudioData::AudioData(const char *fname)
{
  ofstream waveFile; // Wave file
  waveFile.open(fname); // Open .wav file
  if(!waveFile.is_open())
    error("File cannot be opened");

  char header[44]; // Array to store file header
  waveFile.read(header, 44); // Read in file header

  unsigned short chanCount = *reinterpret_cast<unsigned short*>(header + 22); // Get the channel count
  unsigned samplingRate = *reinterpret_cast<unsigned*>(header + 24); // Get the sampling rate
  unsigned short bytesPerFrame = *reinterpret_cast<unsigned short*>(header + 32); // Get the bytes per frame
  unsigned dataSize = *reinterpret_cast<unsigned*>(header + 40); // Get the data size
  unsigned frameCount = dataSize / bytesPerFrame; // Frame count is bytes of data / bytes per frame

  short *data = new short[dataSize / 2]; // Point to data
  waveFile.read(reinterpret_cast<char*>(data), dataSize); // Read in data

  // Set private members
  frame_count = frameCount;
  sampling_rate = samplingRate;
  channel_count = chanCount;
  fdata(frame_count * channel_count);
  for(unsigned i = 0; i < fdata.size(); ++i)
  {
    fdata[i] = data[i];
  }
}

// Writes a wave file
bool waveWrite(const char *fname, const AudioData &ad, unsigned bits)
{

}
