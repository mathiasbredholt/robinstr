
#include "fft.h"
#include "arduino.h"

#define ARM_MATH_CM3
#include "arm_math.h"

#define TEST_LENGTH_SAMPLES 1024

/* -------------------------------------------------------------------
* External Input and Output buffer Declarations for FFT Bin Example
* ------------------------------------------------------------------- */
extern q15_t testInput[TEST_LENGTH_SAMPLES];
static q15_t testOutput[TEST_LENGTH_SAMPLES / 2];

/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t fftSize = 256;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 0;

/* Reference index at which max energy of bin ocuurs */
uint32_t refIndex = 213, testIndex = 0;

/* ----------------------------------------------------------------------
* Max magnitude FFT Bin test
* ------------------------------------------------------------------- */

void fft_test()
{

	arm_status status;
	arm_cfft_radix4_instance_q15 S;
	q15_t maxValue;

	status = ARM_MATH_SUCCESS;

	int timeBefore = millis();

	/* Initialize the CFFT/CIFFT module */
	status = arm_cfft_radix4_init_q15(&S, fftSize,
	                                  ifftFlag, doBitReverse);

	Serial.println(millis() - timeBefore);

	/* Process the data through the CFFT/CIFFT module */
	arm_cfft_radix4_q15(&S, testInput);

	Serial.println(millis() - timeBefore);


	/* Process the data through the Complex Magnitude Module for
	calculating the magnitude at each bin */
	arm_cmplx_mag_q15(testInput, testOutput,
	                  fftSize);

	Serial.println(millis() - timeBefore);

	// /* Calculates maxValue and returns corresponding BIN value */
	// arm_max_f32(testOutput, fftSize, &maxValue, &testIndex);

	// if (testIndex !=  refIndex)
	// {
	// 	status = ARM_MATH_TEST_FAILURE;
	// }

	for (int i = 0; i < TEST_LENGTH_SAMPLES / 2; ++i)
	{
		Serial.write((char) testOutput[i]);
	}

	/* ----------------------------------------------------------------------
	** Loop here if the signals fail the PASS check.
	** This denotes a test failure
	** ------------------------------------------------------------------- */

	// if ( status != ARM_MATH_SUCCESS)
	// {
	// 	while (1);
	// }

	// return maxValue;

	// while (1);                            /* main function does not return */


}
short * find_peaks(testOutput[])
{
	short max = {0,0,0,0};
	// find index of the frequency with greatest amplitude
	for (int i = 1; i < TEST_LENGTH_SAMPLES/2-1; ++i)
	{
		for (int i = 0; j < 4; ++i)
		{
			if (testOutput[i] > testOutput[max[j]])
			{
			max[j] = i;
			}
		}
		if (testOutput[i] > max)
		{
			max = i;
		}
	}
}


double pitch_error(testOutput[], pitch_goal)
{
	find_peaks(testOutput[])
	// Translate i into actual frequency
	for (int i = 0; i < 4; ++i)
	{
		max[i] = SampleRate/2/(TEST_LENGTH_SAMPLES/2) * max[i]; // lav til fixed point

	}
	
	/*
	while (max/pitch > 2) //scale down if a higher order of harmonics is greatest in amplitude
	{
		max /=2;
	}
	while (pitch/max > 2) // scale down...
	{
		max *= 2;
	}

	return max;
	*/
}