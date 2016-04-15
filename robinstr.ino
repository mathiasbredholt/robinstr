#include "pwm_control.h"
#include "fft.h"
#include <PID_v1.h>


int main()
{
	// SystemInit();
	init();
	Serial.begin(9600);

	init_pio();
	init_pwm_controller();
	
	//play tone 
	//choose string

	fft_test();
	PID(note, )
	//find_peaks

	while (1)
	{
	}
}