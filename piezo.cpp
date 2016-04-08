#include "piezo.h"

q15_t buffer[BUFFER_SIZE];
short buffer_index = 0;

bool PIEZO_INTERRUPT;

ADC_DAC_Timer ADC_DAC_Timer;

void init_piezo()
{
    PIEZO_INTERRUPT = false;

    // // From variant.cpp - init() - WIRING LIBRARY
    // pmc_enable_periph_clk(ID_ADC);
    // adc_init(ADC, SystemCoreClock, SAMPLING_FREQ, ADC_STARTUP_FAST);
    // adc_disable_interrupt(ADC, 0xFFFFFFFF);
    // adc_set_resolution(ADC, ADC_12_BITS);
    // adc_configure_power_save(ADC, 0, 0); // Disable sleep
    // adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
    // adc_set_bias_current(ADC, 0); // Bias current - maximum performance over current consumption
    // adc_stop_sequencer(ADC); // not using it
    // adc_disable_tag(ADC); // it has to do with sequencer, not using it
    // adc_disable_ts(ADC); // disable temperature sensor
    // adc_disable_channel_differential_input(ADC, ADC_CHANNEL_0);
    // adc_configure_trigger(ADC, ADC_TRIG_SW, 1); // triggering from software, freerunning mode
    // adc_disable_all_channel(ADC);
    // adc_enable_channel(ADC, ADC_CHANNEL_0);
    // adc_enable_interrupt(ADC, ADC_IER_DRDY);            // Data Ready Interrupt Enable
    // NVIC_EnableIRQ(ADC_IRQn);
    // adc_start(ADC);

    ADC_DAC_Timer.set_sample_rate(1024);  // set sample rate to 1024 Hz
    ADC_DAC_Timer.adc0_setup();
    ADC->ADC_CHER = 0xFF;                  // enable all analog input channels (for use with variable ratio, attack, etc)

}

void ADC_Handler()
{
    if (ADC->ADC_ISR & ADC_ISR_EOC7) { // initialize when ADC end-of-conversion is flagged (NOTE: pin A0 is AD7, so the EOC flag is on EOC7)
        // get conversion result from the ADC CRD. (NOTE: pin A0 is AD7, so conversion data is held in CRD7)
        buffer[buffer_index & MAX_INDEX] = *(ADC->ADC_CDR + 7);
        buffer_index++;

        if (buffer_index & MAX_INDEX == MAX_INDEX) {
            PIEZO_INTERRUPT = true;
        }
    }
}