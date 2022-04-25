// File Name: AudioData.cpp
// Author: Akshat Madan
// Course: CS245
// Project: Assignment 02
// Date: 01/18/2022

#include "AudioData.h"
#include <math.h> // powf, fabs

AudioData::AudioData(unsigned nframes, unsigned R, unsigned nchannels) : frame_count(nframes), sampling_rate(R), channel_count(nchannels)
{
  for(unsigned i = 0; i < nframes * nchannels; ++i)
    fdata.push_back(0); // Initialize vector
}

// Find sample value at given frame and channel, return as value
float AudioData::sample(unsigned frame, unsigned channel) const
{
  return fdata[(frame * channel_count) + channel]; // Return data at required index
}

// Find sample value at given frame and channel, return as reference
float& AudioData::sample(unsigned frame, unsigned channel)
{
  return fdata[(frame * channel_count) + channel]; // Return data at required index
}

// Helper function to get highest absolute value in ad
float highest_abs(AudioData &ad)
{
  unsigned fcount = ad.frames();
  unsigned ccount = ad.channels();
  float max = 0; // Maximum absolute value
  for(unsigned i = 0; i < fcount; ++i)
    for(unsigned j = 0; j < ccount; ++j)
      if(fabs(ad.sample(i,j)) > max)
        max = fabs(ad.sample(i,j)); // Set max, if higher

  return max;
}

// Normalize audio data in place
void normalize(AudioData &ad, float dB)
{
  unsigned fcount = ad.frames();
  unsigned ccount = ad.channels();
  std::vector<float> channel_sums; // Stores the sum of values for each channel

  float tmax = powf(10, (dB / 20.0)); // Target maximum

  for(unsigned i = 0; i < ccount; ++i)
    channel_sums.push_back(0); // Initialize vector

  for(unsigned i = 0; i < fcount; ++i)
    for(unsigned j = 0; j < ccount; ++j)
      channel_sums[j] += ad.sample(i,j); // Get sample at given frame & channel, add to sum for each channel

  for(unsigned i = 0; i < ccount; ++i)
    channel_sums[i] = channel_sums[i] / (float) fcount; // Calculate DC offset for each channel

  for(unsigned i = 0; i < fcount; ++i)
    for(unsigned j = 0; j < ccount; ++j)
      ad.sample(i,j) = ad.sample(i,j) - channel_sums[j]; // Remove DC offset from each channel

  float m = tmax / highest_abs(ad); // Get scaling factor
  
  for(unsigned i = 0; i < fcount; ++i)
    for(unsigned j = 0; j < ccount; ++j)
      ad.sample(i,j) = ad.sample(i,j) * m; // Scale all values
}
