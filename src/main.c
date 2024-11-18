#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/adc.h"
#include "libopencm3/stm32/dma.h"
#include "lcd.h"
#include <stdio.h>

#define SAMPLE_TIME_CYCLES ADC_SMPR_SMP_28DOT5CYC
#define CONSTANT_TO_PERCENTAGE (24.42e-3) / N_DATA
#define N_DATA 10

static volatile uint16_t potBuff0[N_DATA];  // Buffer for ADC0 readings
static volatile uint16_t potBuff1[N_DATA];  // Buffer for ADC1 readings



// Prototipos de las funciones
void config_adc_dma(void);
void config_gpio(void);
float pot_get_value(uint8_t channel);
void pot_leds_control(void);
void update_display(void);



void config_adc_dma(void) {
    // Enable peripheral clocks
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_periph_clock_enable(RCC_DMA1);

    // Configure GPIOA pins as analog inputs for ADC channels
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, GPIO0 | GPIO1);

    // Configure DMA1 Channel 1 for ADC0
    dma_channel_reset(DMA1, DMA_CHANNEL1);
    dma_set_priority(DMA1, DMA_CHANNEL1, DMA_CCR_PL_VERY_HIGH);
    dma_set_peripheral_address(DMA1, DMA_CHANNEL1, (uint32_t)&ADC_DR(ADC1));
    dma_set_memory_address(DMA1, DMA_CHANNEL1, (uint32_t)potBuff0);
    dma_set_number_of_data(DMA1, DMA_CHANNEL1, N_DATA);
    dma_set_memory_size(DMA1, DMA_CHANNEL1, DMA_CCR_MSIZE_16BIT);
    dma_set_peripheral_size(DMA1, DMA_CHANNEL1, DMA_CCR_PSIZE_16BIT);
    dma_enable_circular_mode(DMA1, DMA_CHANNEL1);
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL1);
    dma_set_read_from_peripheral(DMA1, DMA_CHANNEL1);
    dma_enable_channel(DMA1, DMA_CHANNEL1);

    // Configure DMA1 Channel 2 for ADC1
    dma_channel_reset(DMA1, DMA_CHANNEL2);
    dma_set_priority(DMA1, DMA_CHANNEL2, DMA_CCR_PL_HIGH);
    dma_set_peripheral_address(DMA1, DMA_CHANNEL2, (uint32_t)&ADC_DR(ADC1));
    dma_set_memory_address(DMA1, DMA_CHANNEL2, (uint32_t)potBuff1);
    dma_set_number_of_data(DMA1, DMA_CHANNEL2, N_DATA);
    dma_set_memory_size(DMA1, DMA_CHANNEL2, DMA_CCR_MSIZE_16BIT);
    dma_set_peripheral_size(DMA1, DMA_CHANNEL2, DMA_CCR_PSIZE_16BIT);
    dma_enable_circular_mode(DMA1, DMA_CHANNEL2);
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL2);
    dma_set_read_from_peripheral(DMA1, DMA_CHANNEL2);
    dma_enable_channel(DMA1, DMA_CHANNEL2);

    // ADC setup
    adc_power_off(ADC1);
    adc_enable_dma(ADC1);
    adc_disable_eoc_interrupt(ADC1);
    adc_enable_scan_mode(ADC1);  // Enable scan mode for multi-channel reading
    adc_disable_temperature_sensor();
    adc_set_continuous_conversion_mode(ADC1);
    adc_set_right_aligned(ADC1);

    // Set up the ADC channels and sample times
    uint8_t channels[] = {ADC_CHANNEL0, ADC_CHANNEL1};  // Channel sequence for A0 and A1
    adc_set_regular_sequence(ADC1, 2, channels);
    adc_set_sample_time(ADC1, ADC_CHANNEL0, SAMPLE_TIME_CYCLES);
    adc_set_sample_time(ADC1, ADC_CHANNEL1, SAMPLE_TIME_CYCLES);

    // Power on and calibrate ADC
    adc_power_on(ADC1);
    adc_reset_calibration(ADC1);
    adc_calibrate(ADC1);
    while (adc_is_calibrating(ADC1));

    adc_enable_external_trigger_regular(ADC1, ADC_CR2_EXTSEL_SWSTART);
    adc_start_conversion_regular(ADC1);
}

float pot_get_value(uint8_t channel) {
    uint32_t ac = 0;

    // Choose the correct buffer based on the ADC channel
    const volatile uint16_t *buffer = (channel == ADC_CHANNEL0) ? potBuff0 : potBuff1;

    // Sum values in the buffer
    for (uint8_t i = 0; i < N_DATA; i++) {
        ac += buffer[i];
    }

    return (float)ac * CONSTANT_TO_PERCENTAGE;
}

void pot_leds_control(void) {
    float potValue0 = pot_get_value(ADC_CHANNEL0);  // Get the value for ADC0
    float potValue1 = pot_get_value(ADC_CHANNEL1);  // Get the value for ADC1

    // LED control logic based on ADC0 (A0)
    if (potValue0 < 50.0) {
        gpio_set(GPIOA, GPIO10);  // Green LED
    } else {
        gpio_clear(GPIOA, GPIO10);
    }

    if (potValue0 > 50.0) {
        gpio_set(GPIOA, GPIO11);  // Orange LED
    } else {
        gpio_clear(GPIOA, GPIO11);
    }

    if (potValue0 > 75.0) {
        gpio_set(GPIOA, GPIO12);  // Red LED
    } else {
        gpio_clear(GPIOA, GPIO12);
    }
    
    // Additional LED control or other actions based on ADC1 (A1) values can be added here
}

    void config_gpio (void){
    
        /** Enable peripheral clocks for GPIOA */
        rcc_periph_clock_enable(RCC_GPIOA);
    
        /** Configure the GPIO pins for the LEDs */
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO10); /** Green LED */
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO11); /** Orange LED */
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12); /** Red LED */


    }


void update_display() {
    char line[17];

    // Show ADC0 value on the first line
    snprintf(line, sizeof(line), "Volt A0: %u V", (unsigned int)pot_get_value(ADC_CHANNEL0));
    lcd_set_cursor(0, 0);
    lcd_print_string(line);

    // Show ADC1 value on the second line
    snprintf(line, sizeof(line), "Volt A1: %u V", (unsigned int)pot_get_value(ADC_CHANNEL1));
    lcd_set_cursor(1, 0);
    lcd_print_string(line);

    // Additional lines for other values can be added if needed
}

int main(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    config_gpio();
    config_adc_dma();
    lcd_init();

    while (1) {
        pot_leds_control();
        update_display();
    }

    return 0;
}
