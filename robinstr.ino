#include "pwm_control.h"

// int strings[4] = {40, 47, 54, 61}; // init string tunings to E, B, F#, Db

void setup() {
    SerialUSB.println("Welcome to ROBINSTR v0.1");
    init_pwm_controller();
}

void loop() {
    get_data();
}

void get_data()
{
    if (SerialUSB.available() > 1) {
        char chan = SerialUSB.read();
        char val = SerialUSB.read();
        set_pwm_duty((int) chan, (int) val);
    }
}
