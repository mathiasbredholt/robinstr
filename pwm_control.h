#include "sam.h"

void init_pio();
void init_pwm_controller();
void set_pwm_period(uint32_t chan, uint16_t period);
void set_pwm_duty(uint32_t chan, uint16_t duty);