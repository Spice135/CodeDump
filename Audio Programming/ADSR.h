// ADSR.h
// -- DLS style ADSR envelope
// cs245 6/18

#ifndef CS245_ADSR_H
#define CS245_ADSR_H


class ADSR {
  public:
    enum Mode { ATTACK=0, DECAY=1, SUSTAIN=2, RELEASE=3 };
    ADSR(float a=0, float d=0, float s=1, float r=0, float R=44100);
    void sustainOff(void);
    void resetTime(void);
    float getValue(void);
    void incrementTime(void);
    Mode getMode(void);
  private:
    Mode mode;
    float envelope,
          attack_increment,
          decay_factor,
          sustain_level,
          release_factor;
};


#endif

