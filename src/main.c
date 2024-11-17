#include "lcd.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/i2c.h"
#include <stdio.h> // Para sprintf





int main(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    lcd_init();
    lcd_clear();

    while (1) {
        update_display();
        delay_ms(1000);
    }

    return 0;
}