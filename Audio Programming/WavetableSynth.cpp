// File Name: WavetableSynth.cpp
// Author: Akshat Madan
// Course: CS245
// Project: Assignment 09
// Date: 04/13/2022
#include <cmath> // powf
#include <stdexcept> // throw
#include "WavetableSynth.h" // Class definition
#include <iostream>

#define a440 0.98876f // Factor to convert .wav to 440 Hz
#define loop_bgn 14022 // Loop begin for .wav
#define loop_end 21191 // Loop end for .wav

// Class constructor
WavetableSynth::WavetableSynth(int devno, int R) : MidiIn(devno)
{
  note_index1 = -1; // Invalid note index
  frequency1 = 0.0f; // No Frequency

  note_index2 = -1; // Invalid note index
  frequency2 = 0.0f; // No Frequency

  volume = 1.0f; // Volume (Initially set to unity gain)

  // Modulation
  modulation = 0.0f; // No modulation to start
  ti = 1.0f / (float)R; // Time increment step
  t = 0.0f; // Current time

  playing_note1 = false;
  playing_note2 = false;
  firstPlayed = 0; // First note played

  start(); // Start polling
  ad_data = new AudioData("WavetableSynth.wav"); // Open .wav file (driver catches exception)
  resampled_data1 = new Resample(ad_data, 0, a440 * ((float)ad_data->rate() / (float)R), loop_bgn, loop_end); // Resample data to match frequency and loop
  resampled_data2 = new Resample(ad_data, 0, a440 * ((float)ad_data->rate() / (float)R), loop_bgn, loop_end); // Resample data to match frequency and loop
  adsr_note1 = new ADSR(0, 0, 1.0f, 2.0f, R); // Create ADSR envelope for note 1
  adsr_note2 = new ADSR(0, 0, 1.0f, 2.0f, R); // Create ADSR envelope for note 2
}

// Class destructor
WavetableSynth::~WavetableSynth(void)
{
  stop(); // Stop polling
  delete resampled_data1; // Delete created resample first (references audio data)
  delete resampled_data2;
  delete ad_data; // Delete created audio data
}

// Gets the value at current time
float WavetableSynth::getValue(void)
{
  float v1 = adsr_note1->getValue(); // Get ADSR envelope value for note 1
  float v2 = adsr_note2->getValue(); // Get ADSR envelope value for note 2
  float y1 = ((v1 > 0 && adsr_note1->getMode() == ADSR::RELEASE) || playing_note1) ? resampled_data1->getValue() : 0; // If note1 is playing, or is in the release phase get its value
  float y2 = ((v2 > 0 && adsr_note2->getMode() == ADSR::RELEASE) || playing_note2) ? resampled_data2->getValue() : 0; // If note2 is playing, or is in the release phase get its value

  if(modulation == 0.0f)
    return volume * ((y1 * v1) + (y2 * v2)); // Scale by volume and return value at current time
  else
    return cos(acos(y1 * v1) + (modulation * sin(2.0f * 3.1415f * 5.0f * t))); // Modulate frequency
}

// Increments time by 1 step
void WavetableSynth::incrementTime(void)
{
  // Increment time for both samples & ADSR envelope time
  resampled_data1->incrementTime();
  resampled_data2->incrementTime();
  adsr_note1->incrementTime();
  adsr_note2->incrementTime();
  t += ti; // Increment time by time increment
}

// Note on message
void WavetableSynth::onNoteOn(int channel, int note, int velocity)
{
  if(playing_note1 == false || (playing_note2 != false && firstPlayed == 1)) // Play first note
  {
    note_index1 = note;
    frequency1 = powf(2, (float)(note_index1 - 69) / 12.0f); // Calculate note frequency
    frequency1 = 1200.0f * log2f(frequency1); // Frequency ratio in cents
    resampled_data1->setPitchOffset(frequency1);
    playing_note1 = true;
    adsr_note1->resetTime(); // Reset ADSR loop time

    if(playing_note2)
      firstPlayed = 2; // Note 2 was played first
    else
      firstPlayed = 1; // This note was played first
  }
  else // Play / Steal second note
  {
    note_index2 = note;
    frequency2 = powf(2, (float)(note_index2 - 69) / 12.0f); // Calculate note frequency
    frequency2 = 1200.0f * log2f(frequency2); // Frequency ratio in cents
    resampled_data2->setPitchOffset(frequency2);
    adsr_note2->resetTime(); // Reset ADSR loop time

    playing_note2 = true;
    firstPlayed = 1; // Note 1 was played first
  }
}

// Note off message
void WavetableSynth::onNoteOff(int channel, int note)
{
  if (note_index1 == note && playing_note1) // If note off matches curent note
  {
    note_index1 = note; // Switch to current note (for completion's sake)
    frequency1 = 0.0f; // Nothing playing
    playing_note1 = false;
    adsr_note1->sustainOff(); // Release
  }
  else if (note_index2 == note && playing_note2) // If note off matches curent note
  {
    note_index2 = note; // Switch to current note (for completion's sake)
    frequency2 = 0.0f; // Nothing playing
    playing_note2 = false;
    adsr_note2->sustainOff(); // Release
  }
}

// Change in pitch message
void WavetableSynth::onPitchWheelChange(int channel, float value)
{
  //shift = powf(2, (float)(value * 200) / 1200.0f); // Calculate pitch shift
  float shift1 = powf(2, (value * 200) / 1200.0f) * powf(2, frequency1 / 1200.0f);
  shift1 = 1200.0f * log2f(shift1);

  float shift2 = powf(2, (value * 200) / 1200.0f) * powf(2, frequency2 / 1200.0f);
  shift2 = 1200.0f * log2f(shift2);

  resampled_data1->setPitchOffset(shift1); // Value in Cents
  resampled_data2->setPitchOffset(shift2); // Value in Cents
}

// Change in volume
void WavetableSynth::onVolumeChange(int channel, int level)
{
  // Convert from 0 - 127 to float value
  volume = (float)level / 127.0f;
}

// Change in modulation
void WavetableSynth::onModulationWheelChange(int channel, int value)
{
  modulation = (float)value / 127.0f; // Calculate modulation as a float
}
