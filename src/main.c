#include "refresh.h"






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

