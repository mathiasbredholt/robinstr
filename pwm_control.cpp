/*
--- PWM CONTROL ---
PWM0: PIN34
PWM1: PIN36
PWM2: PIN38
PWM3: PIN40
PWM4: PIN9
PWM5: PIN8
PWM6: PIN7
PWM7: PIN6
*/

#include "pwm_control.h"

#define _BIT(bit) (0x1u << bit)

void init_pwm_controller()
{
  //Because we are using PORTC.PIN23 in peripheral B mode
  //  we need to enable the clock for that line.
  PMC->PMC_PCER0 = _BIT(ID_PIOC);

  //Disable PIO Control PC#
  PIOC->PIO_PDR = PIO_PC2;
  PIOC->PIO_PDR |= PIO_PC4;
  PIOC->PIO_PDR |= PIO_PC6;
  PIOC->PIO_PDR |= PIO_PC8;
  PIOC->PIO_PDR |= PIO_PC21;
  PIOC->PIO_PDR |= PIO_PC22;
  PIOC->PIO_PDR |= PIO_PC23;
  PIOC->PIO_PDR |= PIO_PC24;

  // and set up for Peripheral B PWML#
  PIOC->PIO_ABSR = PIO_PC2;
  PIOC->PIO_ABSR |= PIO_PC4;
  PIOC->PIO_ABSR |= PIO_PC6;
  PIOC->PIO_ABSR |= PIO_PC8;
  PIOC->PIO_ABSR |= PIO_PC21;
  PIOC->PIO_ABSR |= PIO_PC22;
  PIOC->PIO_ABSR |= PIO_PC23;
  PIOC->PIO_ABSR |= PIO_PC24;

  //Enable output on C#
  PIOC->PIO_OER = PIO_PC2;
  PIOC->PIO_OER |= PIO_PC4;
  PIOC->PIO_OER |= PIO_PC6;
  PIOC->PIO_OER |= PIO_PC8;
  PIOC->PIO_OER |= PIO_PC21;
  PIOC->PIO_OER |= PIO_PC22;
  PIOC->PIO_OER |= PIO_PC23;
  PIOC->PIO_OER |= PIO_PC24;

  //Enable pull-up on pin
  PIOC->PIO_PUDR = PIO_PC2;
  PIOC->PIO_PUDR |= PIO_PC4;
  PIOC->PIO_PUDR |= PIO_PC6;
  PIOC->PIO_PUDR |= PIO_PC8;
  PIOC->PIO_PUDR |= PIO_PC21;
  PIOC->PIO_PUDR |= PIO_PC22;
  PIOC->PIO_PUDR |= PIO_PC23;
  PIOC->PIO_PUDR |= PIO_PC24;

  //Enable the PWM clock (36)
  PMC->PMC_PCER1 = _BIT((ID_PWM - 32));

  //Channel Prescaler - Use ClockA as configured in PWM_CLK
  //Preliminary frequency = 84000000 / 16 = 5250000Hz
  PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_16;
  PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_16;
  PWM->PWM_CH_NUM[2].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_16;
  PWM->PWM_CH_NUM[3].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_16;
  PWM->PWM_CH_NUM[4].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_16;
  PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_16;
  PWM->PWM_CH_NUM[6].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_16;
  PWM->PWM_CH_NUM[7].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_16;

  //Final frequency = 5250000 / 256 = 20.507kHz
  set_pwm_period(0, 256);
  set_pwm_period(1, 256);
  set_pwm_period(2, 256);
  set_pwm_period(3, 256);
  set_pwm_period(4, 256);
  set_pwm_period(5, 256);
  set_pwm_period(6, 256);
  set_pwm_period(7, 256);

  //Duty cycle = (128 / 256) * 100 = 50%
  set_pwm_duty(0, 0);
  set_pwm_duty(1, 0);
  set_pwm_duty(2, 0);
  set_pwm_duty(3, 0);
  set_pwm_duty(4, 0);
  set_pwm_duty(5, 0);
  set_pwm_duty(6, 0);
  set_pwm_duty(7, 0);

  PWM->PWM_ENA = 0xff;
}

void set_pwm_period(uint32_t chan, uint16_t period)
{
  //If the channel is disabled then we can write directly to the register
  //  else if enabled we write to the update register which acts as a double buffer
  if ((PWM->PWM_SR & _BIT(chan)) == 0)
    PWM->PWM_CH_NUM[chan].PWM_CPRD = period;
  else
    PWM->PWM_CH_NUM[chan].PWM_CPRDUPD = period;
}

void set_pwm_duty(uint32_t chan, uint16_t duty)
{
  //If the channel is disabled then we can write directly to the register
  //  else if enabled we write to the update register which acts as a double buffer
  if ((PWM->PWM_SR & _BIT(chan)) == 0)
    PWM->PWM_CH_NUM[chan].PWM_CDTY = duty;
  else
    PWM->PWM_CH_NUM[chan].PWM_CDTYUPD = duty;
}