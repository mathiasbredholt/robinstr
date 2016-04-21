#include "Arduino.h"
#include "pwm_control.h"

int strings = {40,47,54,61} // init string tunings to E, B, F#, Db

int main()
{
    init();
    Serial.begin(115200);

    Serial.println("Welcome to ROBINSTR v0.1");

    init_pwm_controller();

    while (1)
    {
    	message = readmidi

    }
}
