#include "lcd.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/i2c.h"

/**
 * @brief Configura el periférico I2C1 y los pines SDA/SCL en el puerto B.
 */
void i2c_setup(void) {
    /* Habilitar reloj para GPIOB e I2C1 */
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);

    /* Configurar pines SDA y SCL (GPIOB6 y GPIOB7) para I2C en modo alternativo */
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO6 | GPIO7);

    /* Configurar el I2C a 100kHz */
    i2c_peripheral_disable(I2C1);
    i2c_set_standard_mode(I2C1);
    i2c_set_clock_frequency(I2C1, 36);  // Asume una frecuencia de reloj de 36 MHz
    i2c_set_trise(I2C1, 37);            // Tiempo de subida
    i2c_set_ccr(I2C1, 180);             // Configuración para 100kHz
    i2c_peripheral_enable(I2C1);        // Habilitar el periférico I2C1
}

/**
 * @brief Hace que la letra "A" recorra la pantalla LCD 16x4 de izquierda a derecha, fila por fila.
 */
void parameters_lcd() {
    uint8_t voltaje = 100;
    uint8_t corriente = 50;
    uint8_t potencia = voltaje * corriente;
    uint8_t fp = 10;

    char line1[17], line2[17], line3[17], line4[17];
    snprintf(line1, sizeof(line1), "Voltaje: %dV", voltaje);
    snprintf(line2, sizeof(line2), "Corriente: %dA", corriente);
    snprintf(line3, sizeof(line3), "Potencia: %dW", potencia);
    snprintf(line4, sizeof(line4), "Fp: %d%%", fp);

    // Realizar desplazamiento de los textos por cada línea
    for (int offset = 0; offset <= strlen(line1); offset++) {
        lcd_clear();

        // Desplazamiento en cada línea
        lcd_set_cursor(0, 0);
        lcd_print_string(line1 + offset);

        lcd_set_cursor(1, 0);
        lcd_print_string(line2 + offset);

        lcd_set_cursor(2, 0);
        lcd_print_string(line3 + offset);

        lcd_set_cursor(3, 0);
        lcd_print_string(line4 + offset);

        delay_ms(300); // Esperar 300 ms para el desplazamiento
    }
}

int main(void) {
    i2c_setup();   /** Configurar el periférico I2C1 y los pines SDA/SCL */
    lcd_init();    /** Inicializar el LCD */
    lcd_clear();   /** Limpiar el LCD */

    while (1) {
        parameters_lcd(); /** Llamar a la función para mover la letra "A" */
    }

    return 0;
}
