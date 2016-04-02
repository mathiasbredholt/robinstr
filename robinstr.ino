#include "Arduino.h"
#include "pwm_control.h"
#include "fft.h"
#include "piezo.h"

#define ARM_MATH_CM3
#include "arm_math.h"

extern bool PIEZO_INTERRUPT;
extern volatile int buffer[BUFFER_SIZE];

int main()
{
    init();
    Serial.begin(9600);

    init_pio();
    init_piezo();
    init_pwm_controller();

    while (1)
    {
        // buffer write finished
        if (PIEZO_INTERRUPT) {
            PIEZO_INTERRUPT = false;

            do_fft((q15_t*) buffer);
        }
    }
}