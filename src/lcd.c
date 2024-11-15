#include "lcd.h"

/** Set the EN line high to latch the command, then set it low */
void lcd_pulse_enable(uint8_t data) {
    uint8_t d = data | LCD_EN; 
    i2c_transfer7(I2C1, PCF8574_ADDRESS, &d, 1, NULL, 0);
    delay_ms(2); // Incremento de retardo para estabilidad

    d = data & ~LCD_EN;
    i2c_transfer7(I2C1, PCF8574_ADDRESS, &d, 1, NULL, 0);
    delay_ms(2); // Incremento de retardo
}

/** Prepare a nibble to be sent to the LCD and call pulse to latch it */
void lcd_send_nibble(uint8_t nibble, uint8_t mode) {
    uint8_t data = (nibble & 0xF0) | mode | LCD_BACKLIGHT;
    lcd_pulse_enable(data);
}

/** Split a byte into two nibbles and send each one to the LCD */
void lcd_send_byte(uint8_t byte, uint8_t mode) {
    lcd_send_nibble(byte & 0xF0, mode);        
    lcd_send_nibble((byte << 4) & 0xF0, mode); 
    delay_ms(2); 
}

/** Initialize the LCD by setting up the I2C, configuring the LCD mode, and clearing the display */
void lcd_init(void) {
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);

    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, SDA_PIN | SCL_PIN);

    i2c_peripheral_disable(I2C1);
    i2c_set_standard_mode(I2C1);
    i2c_set_clock_frequency(I2C1, 36);
    i2c_set_trise(I2C1, 36);
    i2c_set_ccr(I2C1, 180);
    i2c_peripheral_enable(I2C1);

    delay_ms(50);

    lcd_send_nibble(0x30, 0);
    delay_ms(5);
    lcd_send_nibble(0x30, 0);
    delay_ms(5);
    lcd_send_nibble(0x30, 0);
    delay_ms(5);
    lcd_send_nibble(0x20, 0);
    delay_ms(5);

    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS | LCD_4BITMODE, 0);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF, 0);
    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, 0);
    lcd_send_byte(LCD_CLEARDISPLAY, 0);
    delay_ms(2);
}

void lcd_print_char(char c) {
    lcd_send_byte(c, LCD_RS);
}

void lcd_print_string(const char* str) {
    while (*str) {
        lcd_print_char(*str++);
    }
}

void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, 0);
    delay_ms(2);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address;

    switch (row) {
        case 0: address = 0x00 + col; break;
        case 1: address = 0x40 + col; break;
        case 2: address = 0x14 + col; break;
        case 3: address = 0x54 + col; break;
        default: return;
    }

    lcd_send_byte(LCD_SETDDRAMADDR | address, 0);
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; i++) {
        __asm__("nop");
    }
}
