#include "main.h"

#define _BV(bit) (0x1u << bit)
#define CHAN 6

#define CR 13
#define BS 8
#define LF 10

/*
---- IO ----
PWM0 = 34
PWM1 = 36
PWM2 = 38
PWM3 = 40
PWM4 = 9
PWM5 = 8
PWM6 = 7
PWM7 = 6
*/

int main()
{
  SystemInit(); // SAM initialize

  disable_watch_dog();
  init_pio();
  init_pwm_controller();

  while (1)
  {
    // set_pwm_duty(CHAN, 48);
    // for (int i = 0; i < 1000000; i++) {};
    // set_pwm_duty(CHAN, 32);
  }
}

void disable_watch_dog()
{
  WDT->WDT_MR = WDT_MR_WDDIS;
}