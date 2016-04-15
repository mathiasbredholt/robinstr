#include "Arduino.h"
#include "pwm_control.h"

int main()
{
    init();
    Serial.begin(115200);

    Serial.println("Welcome to ROBINSTR v0.1");

    init_piezo();
    // init_pwm_controller();

    int now = millis();

    while (1)
    {

    }
}
