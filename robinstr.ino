#include "pwm_control.h"
#include "fft.h"
#include "piezo.h"


int main()
{
    // SystemInit();
    init();
    Serial.begin(9600);

    init_pio();
    init_piezo();
    init_pwm_controller();
    //fft_test();

    while (1)
    {
        // buffer write finished
        if (PIEZO_INTERRUPT) {
            PIEZO_INTERRUPT = false;

        }
    }
}