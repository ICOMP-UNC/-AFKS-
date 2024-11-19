#include "lcd.h"
#include "sensor_acquisition.h"


void pot_leds_control(void) {
    float potValue0 = pot_get_value(0);  // Get the value for ADC0
    float potValue1 = pot_get_value(1);  // Get the value for ADC1

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
}

void config_gpio(void) {
    // Enable peripheral clocks for GPIOA
    rcc_periph_clock_enable(RCC_GPIOA);

    // Configure the GPIO pins for the LEDs
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO10);  // Green LED
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO11);  // Orange LED
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);  // Red LED
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

