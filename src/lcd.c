/**
 * @file lcd.c
 * @brief Implementation of functions for controlling an LCD over I2C using a PCF8574 I/O expander.
 * 
 * This file contains functions for initializing and controlling an LCD, including sending data,
 * printing characters or strings, clearing the display, and setting the cursor position.
 * 
 * @note This file is intended to be used with its corresponding header file `lcd.h`.
 */

#include "lcd.h"

/**
 * @brief Sends a pulse to the LCD's Enable (EN) pin to latch data.
 * 
 * This function sets the Enable line (EN) high to latch the command, then sets it low.
 * It uses I2C to communicate with the PCF8574 I/O expander connected to the LCD.
 * 
 * @param data The byte to be latched.
 */
void lcd_pulse_enable(uint8_t data) {
    uint8_t d = data | LCD_EN;
    i2c_transfer7(I2C1, PCF8574_ADDRESS, &d, 1, NULL, 0);
    delay_ms(2);

    d = data & ~LCD_EN;
    i2c_transfer7(I2C1, PCF8574_ADDRESS, &d, 1, NULL, 0);
    delay_ms(2);
}

/**
 * @brief Sends a 4-bit nibble to the LCD and latches it.
 * 
 * @param nibble The 4-bit nibble to be sent (upper nibble of a byte).
 * @param mode   The mode of the command (`LCD_RS` for data, `0` for commands).
 */
void lcd_send_nibble(uint8_t nibble, uint8_t mode) {
    uint8_t data = (nibble & 0xF0) | mode | LCD_BACKLIGHT;
    lcd_pulse_enable(data);
}

/**
 * @brief Sends an 8-bit byte to the LCD by splitting it into two 4-bit nibbles.
 * 
 * @param byte The byte to be sent.
 * @param mode The mode of the command (`LCD_RS` for data, `0` for commands).
 */
void lcd_send_byte(uint8_t byte, uint8_t mode) {
    lcd_send_nibble(byte & 0xF0, mode);
    lcd_send_nibble((byte << 4) & 0xF0, mode);
    delay_ms(2);
}

/**
 * @brief Initializes the LCD and configures it for 4-bit mode.
 * 
 * This function sets up the I2C peripheral, initializes the LCD, and configures its
 * display settings (e.g., 2-line display, no cursor blinking). The LCD is cleared after initialization.
 */
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

/**
 * @brief Prints a single character to the LCD at the current cursor position.
 * 
 * @param c The character to print.
 */
void lcd_print_char(char c) {
    lcd_send_byte(c, LCD_RS);
}

/**
 * @brief Prints a string to the LCD starting at the current cursor position.
 * 
 * @param str The null-terminated string to print.
 */
void lcd_print_string(const char* str) {
    while (*str) {
        lcd_print_char(*str++);
    }
}

/**
 * @brief Clears the LCD display.
 * 
 * Sends the `LCD_CLEARDISPLAY` command to the LCD to clear its contents.
 */
void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, 0);
    delay_ms(2);
}

/**
 * @brief Sets the cursor to a specific position on the LCD.
 * 
 * This function calculates the DDRAM address based on the specified row and column
 * and sends the `LCD_SETDDRAMADDR` command to position the cursor.
 * 
 * @param row The row number (0 to 3 for a 4-line display).
 * @param col The column number (0 to max column of the display).
 */
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

/**
 * @brief Delays the execution for a specified number of milliseconds.
 * 
 * @param ms The number of milliseconds to delay.
 */
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; i++) {
        __asm__("nop");
    }
}


void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; i++) {
        __asm__("nop");
    }
}
