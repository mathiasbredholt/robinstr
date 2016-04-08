/*
  ADC_DAC_Timer.h - Library for running ADC and DAC at a specified sample rate on the Ardunio Due
  Created by Robert FK Triggs, July 30th 2014
  Released into the public domain.

*/
#ifndef ADC_DAC_Timer_h
#define ADC_DAC_Timer_h

#include "Arduino.h"

class ADC_DAC_Timer
{
  public:
  ADC_DAC_Timer();
  void set_sample_rate(int Freq);
  void dac0_setup ();
  void adc0_setup ();
  private:
};
#endif  