/**
 * @file refresh.c
 * @brief Implements sensor value acquisition, LCD display updates, and PWM-based LED intensity control.
 * 
 * This file handles reading sensor data, displaying values on an LCD, and adjusting LED intensity
 * based on current readings. It interacts with ADC, LCD, and timer modules.
 */

#include "refresh.h"
#include "lcd.h"
#include "stdio.h"
#include "timer_exti.h"

/**
 * @brief Reads and processes sensor values from the ADC buffer.
 * 
 * Reads ADC samples for the specified channel, calculates the average, and converts
 * the result to a corresponding physical quantity (voltage or current).
 * 
 * @param channel The ADC channel to read:
 *                - `0` for voltage (Volts).
 *                - `1` for current (Amperes).
 * @return The processed sensor value in the corresponding unit.
 */
float get_sensor_values(uint8_t channel) {
    uint32_t ac = 0;

    // Accumulate ADC samples for the specified channel
    for (uint8_t i = 0; i < ADC_SAMPLE_COUNT; i++) {
        ac += ADC_BUFFER[channel + i * ADC_CHANNEL_COUNT]; // Interleaved channel access
    }

    // Calculate the average value
    float average = (float)ac / (float)ADC_SAMPLE_COUNT;

    // Convert the raw ADC value to a physical quantity
    if (channel == 0) {
        // Voltage channel: 0 bits → 0V, 4096 bits → 230V
        return (230.0 / 4096.0) * average;
    } else if (channel == 1) {
        // Current channel: 0 bits → 0A, 4096 bits → 10A
        return (10.0 / 4096.0) * average;
    }

    return 0; // Return 0 for invalid channel
}

/**
 * @brief Updates the LCD with the latest sensor values.
 * 
 * Displays voltage, current, power, and phase shift on the LCD across four lines.
 * - Line 1: Voltage (A0 channel).
 * - Line 2: Current (A1 channel).
 * - Line 3: Calculated power (kW).
 * - Line 4: Average phase shift (ms).
 */
void update_values(void) {
    char line[17];
    float power = get_sensor_values(0) * get_sensor_values(1);

    // Display voltage on the first line
    snprintf(line, sizeof(line), "Volt A0: %u V", (unsigned int)get_sensor_values(0));
    lcd_set_cursor(0, 0);
    lcd_print_string(line);

    // Display current on the second line
    snprintf(line, sizeof(line), "Current: %u A", (unsigned int)get_sensor_values(1));
    lcd_set_cursor(1, 0);
    lcd_print_string(line);

    // Display power on the third line
    snprintf(line, sizeof(line), "Power : %u KW", (unsigned int)power);
    lcd_set_cursor(2, 0);
    lcd_print_string(line);

    // Display phase shift on the fourth line
    snprintf(line, sizeof(line), "Phase : %u mS", (unsigned int)(average_phase_shift() / 10));
    lcd_set_cursor(3, 0);
    lcd_print_string(line);
}

/**
 * @brief Sets the PWM duty cycle for controlling the LED.
 * 
 * Limits the duty cycle to a maximum of 1000 and updates the PWM signal for TIM1 channel 3.
 * 
 * @param duty The desired duty cycle value (0–1000).
 */
void set_pwm_duty_cycle(uint16_t duty) {
    if (duty > 1000) duty = 1000; // Limit duty cycle
    timer_set_oc_value(TIM1, TIM_OC3, duty); // Set the value for TIM1_CH3
}

/**
 * @brief Adjusts the LED intensity based on the current sensor reading.
 * 
 * - Turns the LED off for currents below `CURRENT_MIN`.
 * - Adjusts the LED intensity proportionally for currents between `CURRENT_MIN` and `CURRENT_MAX`.
 * - Sets the LED to maximum intensity for currents exceeding `CURRENT_MAX`.
 */
void adjust_led_intensity(void) {
    float current = get_sensor_values(1); // Read current from channel 1

    if (current < CURRENT_MIN) {
        set_pwm_duty_cycle(0); // Turn LED off
    } else if (current <= CURRENT_MAX) {
        uint16_t duty = (uint16_t)((current - CURRENT_MIN) * 1000 / (CURRENT_MAX - CURRENT_MIN));
        set_pwm_duty_cycle(duty); // Adjust LED intensity
    } else {
        set_pwm_duty_cycle(1000); // Set to maximum intensity
    }
}

    
