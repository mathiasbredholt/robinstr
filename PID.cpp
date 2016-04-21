#include "arduino.h"
#include <PID_v1.h>
#include "fft.h"

/* -------------------------------------------------------------------
* Regulator parameters
* ------------------------------------------------------------------- */

double feedback, output,Kp,tau_i,tau_d, errormargin;
Kp = 1;
tau_i = 30;
tau_d = 30;


PID* init_PID() // called from main
{
	PID regulators[4];
	for (int i = 0; i < 3; ++i)
	{
		PID regulator(&feedback,&output,&ref,Kp,tau_i,tau_d,DIRECT)
		SetSampleTime(0); // sampletime = 0 enables Compute() to control when the regulater is updated.
		tuningPID.setmode(AUTOMATIC); //activate regulator
		regulators[i] = regulator;
	}

	return &regulators[];
}

void PID(double pitch, PID tuningPID) // called from main
{
		threshold = pitch/errormargin; // change to fixed point...
		
		while(feedback > threshold){}
				if (readyflag){ //flag set by fft_test
					tuningPID.Compute() //Evaluate output
				}
		}		

		if (feedback < threshold){
		tuningPID.setmode(Manual); // Stop regulating
	}



}


