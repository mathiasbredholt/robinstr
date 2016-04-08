#include "ADC_DAC_Timer.h"

#define ARM_MATH_CM3
#include "arm_math.h"

#define _BIT(bit) (0x1u << bit)
#define SAMPLING_FREQ 1024 // Hz

#define BUFFER_SIZE 16
#define MAX_INDEX 0xf

void init_piezo();