// Resample.h
// cs245 2022.01

#ifndef CS245_RESAMPLE_H
#define CS245_RESAMPLE_H


#include "AudioData.h"


class Resample {
  public:
    explicit Resample(const AudioData *ad_ptr=0, unsigned channel=0,
                      float factor=1, unsigned loop_bgn=0, unsigned loop_end=0);
    float getValue(void);
    void incrementTime(void);
    void setPitchOffset(float cents);
    void resetLoopPoints(unsigned loop_bgn, unsigned loop_end);
  private:
    const AudioData *audio_data;
    unsigned ichannel;
    double findex;
    float speedup,
          multiplier;
    unsigned iloop_bgn, iloop_end;
};


#endif

