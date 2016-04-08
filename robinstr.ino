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

    Serial.println("Welcome to ROBINSTR v0.1");

    init_piezo();
    init_pwm_controller();

    int now = millis();

    while (1)
    {
        // buffer write finished
        if (PIEZO_INTERRUPT) {
            PIEZO_INTERRUPT = false;

            do_fft((q15_t*) buffer);
            // find_peaks
            // calc_pid
        }

        // if ((millis() - now) & 0xFF)
        // {
        //     // Serial.println(PIEZO_INTERRUPT);
        // }
    }
}