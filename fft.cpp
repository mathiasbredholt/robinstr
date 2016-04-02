#include "fft.h"
#include "arduino.h"

#define ARM_MATH_CM3
#include "arm_math.h"

/* -------------------------------------------------------------------
* External Input and Output buffer Declarations for FFT Bin Example
* ------------------------------------------------------------------- */
extern q15_t testInput[TEST_LENGTH_SAMPLES];
static q15_t output_complex[4096];
static q15_t output_mag[2048];

/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t fftSize = 2048;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 0;


void fft_test()
{

	arm_status status;
	arm_cfft_radix4_instance_q15 S;
	arm_rfft_instance_q15 R;
	q15_t maxValue;

	status = ARM_MATH_SUCCESS;

	int timeBefore = millis();

	/* Initialize the CFFT/CIFFT module */
	// status = arm_cfft_radix4_init_q15(&S, fftSize, ifftFlag, doBitReverse);
	arm_rfft_init_q15(&R, &S, fftSize, ifftFlag, doBitReverse);

	/* Process the data through the CFFT/CIFFT module */
	// arm_cfft_radix4_q15(&S, testInput);
	arm_rfft_q15(&R, testInput, output_complex);


	/* Process the data through the Complex Magnitude Module for
	calculating the magnitude at each bin */
	// arm_cmplx_mag_q15(output_complex, output_mag, 256);
	arm_abs_q15(output_complex, output_mag, 2048);


	for (int i = 0; i < 2048; i++)
	{
		Serial.write((char) output_mag[i] << 8);
		Serial.write((char) output_mag[i] & 0xFF);
	}
}