// File Name: WavetableSynth.h
// Author: Akshat Madan
// Course: CS245
// Project: Assignment 09
// Date: 04/13/2022

#ifndef CS245_WAVETABLESYNTH_H
#define CS245_WAVETABLESYNTH_H

#include "MidiIn.h"
#include "AudioData.h"
#include "Resample.h"
#include "ADSR.h"


class WavetableSynth : private MidiIn
{
  public:
    WavetableSynth(int devno, int R);
    ~WavetableSynth(void);
    float getValue(void);
    void incrementTime(void);

  private:
    int note_index1; // Note 1 index
    float frequency1; // Frequency of note 1

    int note_index2; // Note 2 index
    float frequency2; // Frequency of note 2

    float volume; // Value of volume wheel (0.0 -> 1.0)
    float modulation; // Value of mod wheel (0.0 -> 1.0)
    float t;
    float ti;

    bool playing_note1; // Is note 1 playing?
    bool playing_note2; // Is note 2 playing?
    int firstPlayed; // Which note was first played (for note stealing)

    ADSR* adsr_note1; // ADSR for note 1
    ADSR* adsr_note2; // ADSR for note 2

    Resample* resampled_data1; // Resampled output for note 1
    Resample* resampled_data2; // Resampled output for note 2
    AudioData* ad_data; // Audio data of .wav file

    void onNoteOn(int channel, int note, int velocity) override;
    void onNoteOff(int channel, int note) override;
    void onPitchWheelChange(int channel, float value) override;
    void onVolumeChange(int channel, int level) override;
    void onModulationWheelChange(int channel, int value) override;
};


#endif

