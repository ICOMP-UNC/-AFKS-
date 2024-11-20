/**
 * @file lcd.h
 * @brief LCD driver using STM32 microcontroller and I2C expander PCF8574 for a 16x4 LCD.
 *
 * This file contains functions to initialize and control a 16x4 LCD
 * via I2C protocol, using an STM32 microcontroller and a PCF8574 as an I2C expander.
 */

#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/i2c.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/cm3/systick.h"

/** I2C address of the PCF8574. */
#define PCF8574_ADDRESS 0x27

/** STM32 SDA pin. */
#define SDA_PIN GPIO7

/** STM32 SCL pin. */
#define SCL_PIN GPIO6

// LCD Commands
/** Command to clear the LCD screen. */
#define LCD_CLEARDISPLAY 0x01

/** Command to return the cursor to the home position. */
#define LCD_RETURNHOME 0x02

/** Command to set the LCD entry mode. */
#define LCD_ENTRYMODESET 0x04

/** Command to control the display settings. */
#define LCD_DISPLAYCONTROL 0x08

/** Command to set LCD function parameters. */
#define LCD_FUNCTIONSET 0x20

/** Command to set DDRAM address. */
#define LCD_SETDDRAMADDR 0x80

// Flags for display entry mode
/** Flag for left-to-right text entry mode. */
#define LCD_ENTRYLEFT 0x02

// Flags for display control
/** Flag to turn the display on. */
#define LCD_DISPLAYON 0x04

/** Flag to turn the cursor off. */
#define LCD_CURSOROFF 0x00

/** Flag to disable cursor blinking. */
#define LCD_BLINKOFF 0x00

// Flags for function set
/** Flag to set 4-bit data mode. */
#define LCD_4BITMODE 0x00

/** Flag to set 2-line display mode (used to activate more than one line). */
#define LCD_2LINE 0x08

/** Flag to set 5x8 character dot size. */
#define LCD_5x8DOTS 0x00

// Flags for backlight control
/** Flag to turn the LCD backlight on. */
#define LCD_BACKLIGHT 0x08

/** Flag to turn the LCD backlight off. */
#define LCD_NOBACKLIGHT 0x00

/** Enable bit. */
#define LCD_EN 0B00000100

/** Register select bit. */
#define LCD_RS 0B00000001

/*Frequency of Systick in Hz (1mS)*/
#define SYSTICK_FREQUENCY 1000





/**
 * @brief Initializes the LCD, pins, and I2C communication.
 *
 * Configures the LCD for 4-bit mode, multi-line display (required for 16x4),
 * and enables the backlight if required. It also sets up the peripheral clock,
 * I2C setup, and any other initial settings required for normal operation.
 */
void lcd_init(void);

/**
 * @brief Sends a pulse to enable the LCD.
 *
 * This function sets the enable bit (EN) to latch the data or command
 * currently being sent to the LCD.
 *
 * @param data Data to send along with the enable pulse.
 */
void lcd_pulse_enable(uint8_t data);

/**
 * @brief Sends a nibble (4 bits) of data or command to the LCD.
 *
 * This function sends the higher nibble of a byte in 4-bit mode
 * to the LCD, typically used for initializing or sending data or commands.
 *
 * @param nibble 4-bit data to send.
 * @param mode Mode for the data (0 for command, 1 for data).
 */
void lcd_send_nibble(uint8_t nibble, uint8_t mode);

/**
 * @brief Sends a full byte of data or command to the LCD.
 *
 * This function sends both nibbles of a byte to the LCD in 4-bit mode.
 * It is used to send either data or commands, based on the mode parameter.
 *
 * @param byte 8-bit data to send.
 * @param mode Mode for the data (0 for command, 1 for data).
 */
void lcd_send_byte(uint8_t byte, uint8_t mode);

/**
 * @brief Prints a single character on the LCD.
 *
 * This function sends a single character to be displayed on the LCD
 * at the current cursor position.
 *
 * @param c Character to print.
 */
void lcd_print_char(char c);

/**
 * @brief Prints a string on the LCD.
 *
 * This function sends a null-terminated string to the LCD, printing
 * it starting from the current cursor position.
 *
 * @param str Pointer to the null-terminated string to print.
 */
void lcd_print_string(const char* str);

/**
 * @brief Clears the display data.
 *
 * This function clears any characters on the LCD screen and
 * sets the cursor position to the home position.
 */
void lcd_clear(void);

/**
 * @brief Sets the cursor to a specific position on the LCD.
 *
 * This function moves the cursor to a specified row and column
 * on a 16x4 LCD display.
 *
 * @param row The row number (0 for the first row, 1 for the second row,
 *            2 for the third row, and 3 for the fourth row).
 * @param col The column number (0 to 15 for a 16x4 LCD).
 */
void lcd_set_cursor(uint8_t row, uint8_t col);

/**
 * @brief Block for a specified number of milliseconds.
 * 
 * This function uses the global millisecond counter `system_millis` to
 * implement a blocking delay. The function returns once the specified
 * amount of time has elapsed.
 * 
 * @param ms The delay duration in milliseconds.
 */
void delay_ms(uint32_t ms);


/**
 * @brief Configure the SysTick timer to generate interrupts every 1 ms.
 * 
 * This function initializes the SysTick timer using the AHB clock as the
 * source and sets it to generate periodic interrupts with a frequency
 * defined by `SYSTICK_FREQUENCY`. The SysTick interrupt is also enabled.
 */

void systick_setup(void);


/**
 * @brief SysTick interrupt handler.
 * 
 * This function is called every time the SysTick timer generates an interrupt,
 * which occurs at a rate defined by `SYSTICK_FREQUENCY`. It increments the
 * global millisecond counter `system_millis`.
 */
void sys_tick_handler(void);