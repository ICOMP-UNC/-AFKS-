#include "adc_dma.h"

// Prototipos de las funciones

void config_adc_dma(void) {
    // Enable peripheral clocks
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_periph_clock_enable(RCC_DMA1);

    // Configure GPIOA pins as analog inputs for ADC channels
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, ADC_VOLTAGE_PIN | ADC_CURRENT_PIN);

    // Configure DMA1 Channel 1 for both ADC0 and ADC1 data
    dma_channel_reset(DMA1, DMA_CHANNEL1);
    dma_set_priority(DMA1, DMA_CHANNEL1, DMA_CCR_PL_VERY_HIGH);
    dma_set_peripheral_address(DMA1, DMA_CHANNEL1, (uint32_t)&ADC_DR(ADC1));
    dma_set_memory_address(DMA1, DMA_CHANNEL1, (uint32_t)ADC_BUFFER);
    dma_set_number_of_data(DMA1, DMA_CHANNEL1, ADC_BUFFER_SIZE);
    dma_set_memory_size(DMA1, DMA_CHANNEL1, DMA_CCR_MSIZE_16BIT);
    dma_set_peripheral_size(DMA1, DMA_CHANNEL1, DMA_CCR_PSIZE_16BIT);
    dma_enable_circular_mode(DMA1, DMA_CHANNEL1);
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL1);
    dma_set_read_from_peripheral(DMA1, DMA_CHANNEL1);
    dma_enable_channel(DMA1, DMA_CHANNEL1);

    // ADC setup
    adc_power_off(ADC1);
    adc_enable_dma(ADC1);
    adc_disable_eoc_interrupt(ADC1);
    adc_enable_scan_mode(ADC1);  // Enable scan mode for multi-channel reading
    adc_disable_temperature_sensor();
    adc_set_continuous_conversion_mode(ADC1);
    adc_set_right_aligned(ADC1);

    // Set up the ADC channels and sample times
    uint8_t channels[] = {ADC_VOLTAGE_CHANNEL, ADC_CURRENT_CHANNEL};  // Channel sequence for A0 and A1
    adc_set_regular_sequence(ADC1, ADC_CHANNEL_COUNT, channels);
    adc_set_sample_time(ADC1, ADC_VOLTAGE_CHANNEL, SAMPLE_TIME_CYCLES);
    adc_set_sample_time(ADC1, ADC_CURRENT_CHANNEL, SAMPLE_TIME_CYCLES);

    // Power on and calibrate ADC
    adc_power_on(ADC1);
    adc_reset_calibration(ADC1);
    adc_calibrate(ADC1);
    while (adc_is_calibrating(ADC1));

    adc_enable_external_trigger_regular(ADC1, ADC_CR2_EXTSEL_SWSTART);
    adc_start_conversion_regular(ADC1);
}




