#include "piezo.h"
#include "Arduino.h"
#define ARM_MATH_CM3
#include "arm_math.h"

#define _BIT(bit) (0x1u << bit)
#define SAMPLING_FREQ 1024 // Hz

#define BUFFER_SIZE 16
#define MAX_INDEX 0xf

volatile q15_t buffer[BUFFER_SIZE];
volatile int buffer_index = 0;

bool PIEZO_INTERRUPT;

void init_piezo()
{
    PIEZO_INTERRUPT = false;

    // From variant.cpp - init() - WIRING LIBRARY
    pmc_enable_periph_clk(ID_ADC);
    adc_init(ADC, SystemCoreClock, SAMPLING_FREQ, ADC_STARTUP_FAST);
    adc_disable_interrupt(ADC, 0xFFFFFFFF);
    adc_set_resolution(ADC, ADC_12_BITS);
    adc_configure_power_save(ADC, 0, 0); // Disable sleep
    adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
    adc_set_bias_current(ADC, 1); // Bias current - maximum performance over current consumption
    adc_stop_sequencer(ADC); // not using it
    adc_disable_tag(ADC); // it has to do with sequencer, not using it
    adc_disable_ts(ADC); // deisable temperature sensor
    adc_disable_channel_differential_input(ADC, ADC_CHANNEL_7);
    adc_configure_trigger(ADC, ADC_TRIG_SW, 1); // triggering from software, freerunning mode
    adc_disable_all_channel(ADC);
    adc_enable_channel(ADC, (adc_channel_num_t) 0);
    adc_enable_interrupt(ADC, ADC_IER_DRDY);            // Data Ready Interrupt Enable
    NVIC_EnableIRQ(ADC_IRQn);
    adc_start(ADC);
}

void ADC_Handler()
{
    if ((adc_get_status(ADC) & ADC_ISR_DRDY) == ADC_ISR_DRDY) {
        buffer[buffer_index & MAX_INDEX] = adc_get_latest_value(ADC);
        buffer_index++;

        if (buffer_index & MAX_INDEX == 0) {
            PIEZO_INTERRUPT = true;
        }
    }
}