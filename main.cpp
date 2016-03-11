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

void InitPIO(void);
void InitPWMController_CLKA(void);
void InitPWMController_MCLK(void);

void SetPeriod(uint32_t chan, uint16_t period);
void SetDuty(uint32_t chan, uint16_t duty);

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
  /* Initialize the SAM system */

  SystemInit();

  InitPIO();
//  InitPWMController_CLKA();
  InitPWMController_MCLK();

  while (1)
  {
    serial_demo();
    SetDuty(CHAN, 16);
    PutChar(25);
    for (int i = 0; i < 100000000; i++) {};
    SetDuty(CHAN, 48);
    PutChar(75);
    for (int i = 0; i < 100000000; i++) {};
    SetDuty(CHAN, 32);
    PutChar(50);
  }
}

void init_

void InitPIO()
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

void InitPWMController_CLKA()
{
  //Enable the PWM clock (36)
  PMC->PMC_PCER1 = _BV((ID_PWM - 32));

  //Configure clock
  //Calculating the frequency (PREA x CPRD X DIVA) / MCK
  // freq = (256 X 2) / 84000000 = 164063.5Hz
  PWM->PWM_CLK = 2 | (8 << 8);

  //Channel Prescaler - Use ClockA as configured in PWM_CLK
  PWM->PWM_CH_NUM[CHAN].PWM_CMR = PWM_CMR_CPRE_CLKA;

  //Final frequency = 164063.5 / 255 = 643Hz
  SetPeriod(CHAN, 255);

  //Duty cycle = ratio of Period to Duty (Duty / Period) * 100 = 50%
  SetDuty(CHAN, 128);

  //Enable channel
  PWM->PWM_ENA = _BV(CHAN);
}

void InitPWMController_MCLK()
{
  //Enable the PWM clock (36)
  PMC->PMC_PCER1 = _BV((ID_PWM - 32));

  //Channel Prescaler - Use ClockA as configured in PWM_CLK
  //Preliminary frequency = 84000000 / 16 = 5250000Hz
  PWM->PWM_CH_NUM[CHAN].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_16;

  //Final frequency = 5250000 / 400 = 13125Hz
  SetPeriod(CHAN, 64);

  //Duty cycle = (128 / 400) * 100 = 32%
  SetDuty(CHAN, 32);

  PWM->PWM_ENA = _BV(CHAN);
}

void SetPeriod(uint32_t chan, uint16_t period)
{
  //If the channel is disabled then we can write directly to the register
  //  else if enabled we write to the update register which acts as a double buffer
  if ((PWM->PWM_SR & _BV(chan)) == 0)
    PWM->PWM_CH_NUM[chan].PWM_CPRD = period;
  else
    PWM->PWM_CH_NUM[CHAN].PWM_CPRDUPD = period;
}

void SetDuty(uint32_t chan, uint16_t duty)
{
  //If the channel is disabled then we can write directly to the register
  //  else if enabled we write to the update register which acts as a double buffer
  if ((PWM->PWM_SR & _BV(chan)) == 0)
    PWM->PWM_CH_NUM[chan].PWM_CDTY = duty;
  else
    PWM->PWM_CH_NUM[CHAN].PWM_CDTYUPD = duty;
}


void PutChar(const uint8_t c)
{
  // Check if the transmitter is ready
  while (!(UART->UART_SR & UART_SR_TXRDY)) {}
  // Send the character
  UART->UART_THR = c;
}


