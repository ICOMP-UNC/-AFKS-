#include "lcd.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/i2c.h"
#include <stdio.h> // Para sprintf

// Definición de variables globales
uint32_t voltaje = 220;
uint32_t corriente = 53;
uint32_t factor_potencia = 5;
uint32_t potencia;

void i2c_setup(void) {
    /* Habilitar reloj para GPIOB e I2C1 */
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);

    /* Configurar pines SDA y SCL (GPIOB6 y GPIOB7) para I2C en modo alternativo */
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO6 | GPIO7);

    /* Configurar el I2C a 100kHz */
    i2c_peripheral_disable(I2C1);
    i2c_set_standard_mode(I2C1);
    i2c_set_clock_frequency(I2C1, 36);  // Ajustar si el SYSCLK no es 36 MHz
    i2c_set_trise(I2C1, 37);            // Tiempo de subida recomendado para 100 kHz
    i2c_set_ccr(I2C1, 180);             // Configuración para 100kHz
    i2c_peripheral_enable(I2C1);        // Habilitar el periférico I2C1
}

void update_display() {
    
 char line[17];
    potencia = (voltaje * corriente) / 100; // Calcular potencia en W

    // Mostrar voltaje en la primera línea
    snprintf(line, sizeof(line), "Voltaje: %u V", voltaje);
    lcd_set_cursor(0, 0);
    lcd_print_string(line);

    // Mostrar corriente en la segunda línea
    snprintf(line, sizeof(line), "Corriente: %u A", corriente);
    lcd_set_cursor(1, 0);
    lcd_print_string(line);

    // Mostrar potencia en la tercera línea
    snprintf(line, sizeof(line), "Potencia: %u W", potencia);
    lcd_set_cursor(2, 0);
    lcd_print_string(line);

    // Mostrar factor de potencia en la cuarta línea
    snprintf(line, sizeof(line), "Fp: %u", factor_potencia);
    lcd_set_cursor(3, 0);
    lcd_print_string(line);

}

int main(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    i2c_setup();
    delay_ms(50);
    lcd_init();
    lcd_clear();

    while (1) {
        update_display();
        delay_ms(1000);
    }

    return 0;
}
