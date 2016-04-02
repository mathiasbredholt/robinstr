#define ARM_MATH_CM3
#include "arm_math.h"
#include "piezo.h"

#define FFT_SIZE 2048

void do_fft(q15_t *input_buffer);