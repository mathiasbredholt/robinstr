/*
 * Due_PWM_101_PWMC.c
 *
 * Created: Wed 10 1 2014 4:29:53 PM
 *  Author: Mike
 */

#include "sam.h"

#define _BV(bit) (0x1u << bit)
#define CHAN 6

#define CR 13
#define BS 8
#define LF 10

void init_pio(void);
void init_pwm_controller(void);
void disable_watch_dog();

void set_pwm_period(uint32_t chan, uint16_t period);
void set_pwm_duty(uint32_t chan, uint16_t duty);

int main(void)
{
  SystemInit(); // SAM initialize

  disable_watch_dog();
  init_pio();
  init_pwm_controller();

  while (1)
  {
    set_pwm_duty(CHAN, 32);
    for (int i = 0; i < 100000000000; i++) {};
    set_pwm_duty(CHAN, 48);
    for (int i = 0; i < 100000000000; i++) {};
    set_pwm_duty(CHAN, 32);
  }
}

void disable_watch_dog()
{
  WDT->WDT_MR = WDT_MR_WDDIS;
}

void init_pio()
{
  //Because we are using PORTC.PIN23 in peripheral B mode
  //  we need to enable the clock for that line.
  PMC->PMC_PCER0 = _BV(ID_PIOC);

  //Disable PIO Control on PC23 and set up for Peripheral B PWML7
  //PWM Pin 6 on the board
  PIOC->PIO_PDR = PIO_PC23;
  PIOC->PIO_ABSR = PIO_PC23;

  //Enable output on C23
  PIOC->PIO_OER = PIO_PC23;

  //Enable pull-up on pin
  PIOC->PIO_PUDR = PIO_PC23;
}

void init_pwm_controller()
{
  //Enable the PWM clock (36)
  PMC->PMC_PCER1 = _BV((ID_PWM - 32));

  //Channel Prescaler - Use ClockA as configured in PWM_CLK
  //Preliminary frequency = 84000000 / 16 = 5250000Hz
  PWM->PWM_CH_NUM[CHAN].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_16;

  //Final frequency = 5250000 / 400 = 13125Hz
  set_pwm_period(CHAN, 64);

  //Duty cycle = (128 / 400) * 100 = 32%
  set_pwm_duty(CHAN, 32);

  PWM->PWM_ENA = _BV(CHAN);
}

void set_pwm_period(uint32_t chan, uint16_t period)
{
  //If the channel is disabled then we can write directly to the register
  //  else if enabled we write to the update register which acts as a double buffer
  if ((PWM->PWM_SR & _BV(chan)) == 0)
    PWM->PWM_CH_NUM[chan].PWM_CPRD = period;
  else
    PWM->PWM_CH_NUM[CHAN].PWM_CPRDUPD = period;
}

void set_pwm_duty(uint32_t chan, uint16_t duty)
{
  //If the channel is disabled then we can write directly to the register
  //  else if enabled we write to the update register which acts as a double buffer
  if ((PWM->PWM_SR & _BV(chan)) == 0)
    PWM->PWM_CH_NUM[chan].PWM_CDTY = duty;
  else
    PWM->PWM_CH_NUM[CHAN].PWM_CDTYUPD = duty;
}


