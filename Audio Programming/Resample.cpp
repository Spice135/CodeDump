// File Name: Resample.cpp
// Author: Akshat Madan
// Course: CS245
// Project: Assignment 04
// Date: 02/03/2022

#include "Resample.h"

Resample::Resample(const AudioData *ad_ptr, unsigned channel, float factor, unsigned loop_bgn, unsigned loop_end) : 
                   audio_data(ad_ptr), ichannel(channel), speedup(factor), iloop_bgn(loop_bgn), iloop_end(loop_end) 
{
  findex = 0;
}

float Resample::getValue(void)
{
  if(iloop_bgn > iloop_end && (findex >= audio_data->frames() || findex < 0))
    return 0.0f;

  if(findex >= iloop_end && iloop_bgn < iloop_end) // Calculate looped index if needed
  {
    float rem = fmod((findex - iloop_bgn), (iloop_end - iloop_bgn)); // Remainder
    if(rem < 0)
      rem += iloop_end - iloop_bgn;
    findex = iloop_bgn + rem; // Adjust fractional index if needed
  }

  int lower_bound = static_cast<int>(findex); // Calculate lower bound
  int upper_bound = lower_bound + 1; // Calculate upper bound
  float dec = findex - lower_bound; // Calculate decimal part
  float data_lb = audio_data->sample(lower_bound, ichannel); // Get data at lower bound
  float data_ub = audio_data->sample(upper_bound, ichannel); // Get next data

  return data_lb + (dec * (data_ub - data_lb)); // Interpolated value
}

void Resample::incrementTime(void)
{
  float t = findex / (audio_data->rate() * speedup); // Get time
  t += 1.0f / (float)(audio_data->rate()); // Increment time
  findex = t * speedup * audio_data->rate(); // Calculate new fractional index
}

void Resample::setPitchOffset(float cents)
{
  if(cents == 0)
    return; // Do nothing
  speedup *= powf(2, cents / 1200.0f); // Convert cents to frequency ratio and set new speed up factor
}

void Resample::resetLoopPoints(unsigned loop_bgn, unsigned loop_end)
{
  iloop_bgn = loop_bgn;
  iloop_end = loop_end;

  if(findex >= loop_end) // Calculate looped index if needed
  {
    float rem = fmod((findex - iloop_bgn), (iloop_end - iloop_bgn)); // Remainder
    if(rem < 0)
      rem += iloop_end - iloop_bgn;
    findex = iloop_bgn + rem; // Adjust fractional index if needed
  }
}
