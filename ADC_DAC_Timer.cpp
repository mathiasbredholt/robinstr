/*
  ADC_DAC_Timer - Library for running ADC and DAC at a specified sample rate on the Ardunio Due.
  Create a timer at the specified frequency on TC0 Ch0.
  This library also comes with functions to setup the Due's DAC0 and A0 inputs to use the TC0 timer.

  Created by Robert FK Triggs, July 30th 2014
  https://robert-triggs.squarespace.com/

  Credit to MarkT for his starting point: http://forum.arduino.cc/index.php?topic=205096.0

  Released into the public domain.
*/

#include "Arduino.h"
#include "ADC_DAC_Timer.h"

ADC_DAC_Timer::ADC_DAC_Timer()
{
}

void ADC_DAC_Timer::set_sample_rate(int Freq){
/*Timer created from an int value (in Hz) for the desired sample frequency to be used with ADC and DACs.
  This frequency should be 2x the maximum frequency that will be samples (Nyquist).
  Desired frequency clock is made available on TI0A Ch0

  
******** TABLE OF PARAMETERS FOR CLOCKS ***********

TC      Chan  	NVIC "irq"  	IRQ handler   	PMC id

TC0	0	TC0_IRQn	TC0_Handler	ID_TC0
TC0	1	TC1_IRQn	TC1_Handler	ID_TC1
TC0	2	TC2_IRQn	TC2_Handler	ID_TC2
TC1	0	TC3_IRQn	TC3_Handler	ID_TC3
TC1	1	TC4_IRQn	TC4_Handler	ID_TC4
TC1	2	TC5_IRQn	TC5_Handler	ID_TC5
TC2	0	TC6_IRQn	TC6_Handler	ID_TC6
TC2	1	TC7_IRQn	TC7_Handler	ID_TC7
TC2	2	TC8_IRQn	TC8_Handler	ID_TC8
*/
  
  pmc_enable_periph_clk (ID_TC0) ;  // start up TC0 Channel 0
  TcChannel * t = &(TC0->TC_CHANNEL)[0] ;  // create a pointer to TC0, Chan 0
   
  t->TC_CCR = TC_CCR_CLKDIS ;  // disable internal clocking while setup regs
  t->TC_IDR = 0xFFFFFFFF ;     // disable interrupts
  t->TC_SR ;                   // read int status reg to clear pending
  
  /*Setup clock speed
  TCCLKS selects the timer clock based on a scaler of the 84 MHz MCK
  TIMER_CLOCK1 = MCK/2
  TIMER_CLOCK2 = MCK/8
  TIMER_CLOCK3 = MCK/32
  TIMER_CLOCK4 = MCK/128
  TIMER_CLOCK5(1) = SLCK
  */
  t->TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 |   // set timer clock at 42 MHz
              TC_CMR_WAVE |                  // waveform mode
              TC_CMR_WAVSEL_UP_RC;           // count-up PWM using RegisterC as threshold to reset
  
  
  /*Create a slower clock based on the desired sample rate*/
  
  int cycle = 42000000/Freq;  // 42 MHz/desired frequency, yields the counter used by registers
  t->TC_RC =  cycle ;         // RegisterC counts at the specificed cycle speed (ticks high each cycle)
  t->TC_RA =  cycle/2 ;       // RegisterA counts twice as fast as RC, 
  t->TC_CMR = (t->TC_CMR & 0xFFF0FFFF) |   
              TC_CMR_ACPC_SET |            // RC set compare on TIOA, creating a new clock from RC and RA cycle times
              TC_CMR_ACPA_CLEAR ;          // RA clear on TIOA.
  
  t->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG ;  // re-enable local clocking and switch to hardware trigger source. 
}

void ADC_DAC_Timer::dac0_setup ()
{
/*DAC Setup configures DAC0 output to use TI0A Ch0 as the trigger.
  DACC clock is set at MCK/2 (42MHz) and takes 25 clock periods to provide the analog output by default.
  */
  
  pmc_enable_periph_clk (DACC_INTERFACE_ID) ; // startup the DAC
  DACC->DACC_CR = DACC_CR_SWRST ;             // reset DAC
  DACC->DACC_MR = DACC_MR_USER_SEL_CHANNEL0,  // select channel 0
                  (3 << DACC_MR_STARTUP_Pos) |  // 3 = 24 cycles for startup, plenty of time
                  DACC_MR_TRGEN_EN |          // enable external trigger mode
                  DACC_MR_TRGSEL (1);         //select trigger source
                  
                /*TRGSEL Options
                  0 0 0 External trigger
                  0 0 1 TIO Output of the Timer Counter Channel 0
                  0 1 0 TIO Output of the Timer Counter Channel 1
                  0 1 1 TIO Output of the Timer Counter Channel 2
                  1 0 0 PWM Event Line 0
                  1 0 1 PWM Event Line 1*/
                  
  DACC->DACC_IDR = 0xFFFFFFFF ; // no interrupts
  DACC->DACC_CHER = DACC_CHER_CH0 << 0 ; // enable channel_0
}

void ADC_DAC_Timer::adc0_setup ()
{
/* SETUP ADC to run with TI0A Ch0 as the trigger, with 12 bit accuracy
   Only A0 is activated by this setup, with end of conversion interrupt too
   All other analog inputs disabled, will need reactivating in setup.
*/
  adc_disable_all_channel(ADC); // disable all ADC channels (less noise)
  eAnalogReference(DEFAULT);    // set ADC reference voltage to the 3.3V default on the Due
  NVIC_EnableIRQ (ADC_IRQn) ;   // enable ADC interrupt vector
  ADC->ADC_IDR = 0xFFFFFFFF ;   // disable interrupts
  ADC->ADC_IER = 0x80 ;         // enable AD7 End-Of-Conv interrupt (Arduino pin A0), used to check for completed conversions
  ADC->ADC_CHER = 0x80 ;        // enable channel A0 (AD7)
  
  /*CHANNEL ASSIGNMENTS ARE DIFFERENT TO WHAT IS MARKED ON THE DUE
  A0 = AD7, A1 = AD6, A2 = AD5, A3 = AD4, A4 = AD3, A5 = AD2,
  A6 = AD1, A7 = AD0, A8 = AD10, A9 = AD11, A10 = AD12, A11 = AD13.*/
  
  ADC->ADC_CGR = 0 ;            /* Set all gains to 1
  
  GAINx | Gain DIFFx = 0 | Gain DIFFx = 1
    0 0           1             0.5
    0 1           1               1
    1 0           2               2
    1 1           4               2 */
    
  ADC->ADC_COR = 0x00000000 ;             // All offsets off
  ADC->ADC_MR = ADC_MR_ANACH |            // allow for different analog settings on each channel (if desired)
                ADC_MR_LOWRES_BITS_12 |   // enforce 12 bit capture
                ADC_MR_FREERUN_OFF |      // turn off freerun to force wait for trigger
                ADC_MR_TRGEN |            // enable external trigger mode for ADC
                ADC_MR_TRGSEL_ADC_TRIG1 ; /* ADC_TRIG1 = Use TIOA output from TC0 as the trigger
                
                ADC_TRIG0   External : ADCTRG
                ADC_TRIG1   TIOA Output of TC Channel 0
                ADC_TRIG2   TIOA Output of TC Channel 1
                ADC_TRIG3   TIOA Output of TC Channel 2
                ADC_TRIG4   PWM Event Line 0
                */               
}