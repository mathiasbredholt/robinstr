#include "fft.h"

static q15_t output_complex[FFT_SIZE * 2];
static q15_t output_mag[FFT_SIZE];

void do_fft(q15_t *input_buffer)
{
	arm_cfft_radix4_instance_q15 S;
	arm_rfft_instance_q15 R;
	q15_t maxValue;

	/* Initialize the CFFT/CIFFT module */
	// status = arm_cfft_radix4_init_q15(&S, fftSize, ifftFlag, doBitReverse);
	arm_rfft_init_q15(&R, &S, FFT_SIZE, 0, 0);

	/* Process the data through the CFFT/CIFFT module */
	// arm_cfft_radix4_q15(&S, input_buffer);
	arm_rfft_q15(&R, input_buffer, output_complex);


	/* Process the data through the Complex Magnitude Module for
	calculating the magnitude at each bin */
	// arm_cmplx_mag_q15(output_complex, output_mag, 256);
	arm_abs_q15(output_complex, output_mag, FFT_SIZE);


	// for (int i = 0; i < FFT_SIZE; i++)
	// {
	// 	Serial.write((char) output_mag[i] << 8);
	// 	Serial.write((char) output_mag[i] & 0xFF);
	// }
}