#include "pwm_control.h"
#include "fft.h"


int main()
{
    // SystemInit();
    init();
    Serial.begin(9600);

    init_pio();
    init_pwm_controller();
    //fft_test();
    while (1)
    {
    }
}