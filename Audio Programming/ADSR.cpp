// File Name: ADSR.cpp
// Author: Akshat Madan
// Course: CS245
// Project: Assignment 07
// Date: 03/30/2022

#include "ADSR.h" // Class definition
#include <cmath> // logf, expf

// Constructs the ADSR envelope
ADSR::ADSR(float a, float d, float s, float r, float R)
{
  attack_increment = (a > 0) ? 1.0f / (R * a) : 1; // Attack increment (1 / a*R)

  float decay_rate = (d > 0) ? (96.0f / 20.0f) * (logf(10) / d) : 0; // Decay rate from the parameter given
  decay_factor = expf((-1.0f * decay_rate) / R); // Decay factor from decay rate

  float release_rate = (r > 0) ? (96.0f / 20.0f) * (logf(10) / r) : 0; // Release decay rate from parameter given
  release_factor = expf((-1.0f * release_rate) / R); // Release decay factor from release rate

  // Set starting mode as attack or decay based on attack time
  envelope = (a > 0) ? 0 : 1;
  mode = (envelope == 0) ? ATTACK : DECAY;
  sustain_level = s; // Set sustain level
}

// Ends the sustain phase immediately
void ADSR::sustainOff(void)
{
  mode = RELEASE; // Starts the release phase
}

// Resets time to the start of the envelope
void ADSR::resetTime(void)
{
  envelope = (attack_increment == 1) ? 1 : 0; // If attack_increment is 1 then a must have been <= 0
  mode = (envelope == 0) ? ATTACK : DECAY;
}

// Gets the envelope value at the current time
float ADSR::getValue(void)
{
  return envelope; // Current value
}

// Increments the current time by 1 step
void ADSR::incrementTime(void)
{
  switch(mode) // With fall through
  {
    case ATTACK:
      if(envelope < 1.0f)
      {
        envelope += attack_increment; // Increase linearly
        break;
      }
      else
        mode = DECAY; // Start decay phase

    case DECAY:
      if(envelope > sustain_level)
      {
        envelope *= decay_factor; // Decay exponentially
        break;
      }
      else
        mode = SUSTAIN; // Start sustain phase

    case SUSTAIN:
      envelope = sustain_level; // Set sustain

    case RELEASE:
      envelope *= release_factor; // Decay exponentially
  }
}

// Returns the current mode of the ADSR envelope
ADSR::Mode ADSR::getMode(void)
{
  return mode; // Return the current mode
}
