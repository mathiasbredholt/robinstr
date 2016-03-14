#include "sam.h"
#include "arduino.h"

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

void init_pio();
void init_pwm_controller();
void set_pwm_period(uint32_t chan, uint16_t period);
void set_pwm_duty(uint32_t chan, uint16_t duty);