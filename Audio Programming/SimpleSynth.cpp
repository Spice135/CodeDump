// File Name: SimpleSynth.cpp
// Author: Akshat Madan
// Course: CS245
// Project: Assignment 06
// Date: 03/23/2022

#include <cmath>
#include "SimpleSynth.h"

// Basic sawtooth wave
float sawtooth(float t)
{
  t -= std::floor(t);
  return 2 * t - 1;
}

// Class constructor
SimpleSynth::SimpleSynth(int devno, int R) : MidiIn(devno)
{
  argument = 0; // Current argument
  shift = 1.0f; // No pitch shift to start
  irate = 1.0f / R; // Increment rate
  note_index = -1; // Invalid note index
  frequency = 0.0f; // No Frequency
  start(); // Start polling
}

// Class destructor
SimpleSynth::~SimpleSynth(void)
{
  stop(); // Stop polling
}

// Get's current value
float SimpleSynth::getValue(void)
{
  float y = (note_index >= 0) ? sawtooth(argument) : 0; // Sawtooth wave from example
  return 0.3f * y;
}

// Increment time to next sample
void SimpleSynth::incrementTime(void)
{
  argument += shift * dargument; // Increment argument, apply pitch shift
}

// Note on message
void SimpleSynth::onNoteOn(int channel, int note, int velocity)
{
  note_index = note; // Allow note stealing
  frequency = powf(2, (float)(note_index - 69) / 12.0f) * 440.0f; // Calculate note frequency
  dargument = frequency * irate; // Change in argument
}

// Note off message
void SimpleSynth::onNoteOff(int channel, int note)
{
  if (note_index == note) // If note off matches curent note
  {
    note_index = note; // Switch to current note (for completion's sake)
    frequency = 0.0f; // Nothing playing
    dargument = frequency * irate; // Change in argument
  }
}

// Change in pitch message
void SimpleSynth::onPitchWheelChange(int channel, float value)
{
  shift = powf(2, (float)(value * 200) / 1200.0f); // Calculate pitch shift
}
