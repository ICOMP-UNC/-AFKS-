#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/adc.h"
#include "libopencm3/stm32/dma.h"
#include "libopencm3/stm32/i2c.h"
#include "libopencm3/stm32/timer.h"
#include "libopencm3/stm32/exti.h"
#include "libopencm3/cm3/nvic.h"
#include "adc_dma.h"
#include "lcd.h"
#include "stdio.h"
#include "timer_exti.h"
#include <stdint.h>


/** @brief Minimum current value for PWM scaling. */
#define CURRENT_MIN 7

/** @brief Maximum current value for PWM scaling. */
#define CURRENT_MAX 10

/** @brief True valor declaration. */
#define TRUE 1

/** @brief False valor declaration. */
#define FALSE 0





/**
 * @brief Sets the PWM duty cycle.
 * 
 * Adjusts the PWM signal on PA10 based on a duty cycle value (0-1000).
 * 
 * @param duty Duty cycle value (0-1000).
 */
void set_pwm_duty_cycle(uint16_t duty);

/**
 * @brief Implements the PWM duty cycle. Reads the Current value and adapts
 *  the PWM to the corresponding duty cycle.
 * 
 */
void pwm_implementation(void);


/**
 * @brief Update and display the sensor values and calculated variables on an LCD screen.
 * This function retrieves sensor values for voltage and current, calculates the power, 
 * and displays these values along with the phase shift on a 4-line LCD. The values are
 * formatted as strings for better readability. 
 */
void update_values(void);



/**
 * @brief Reads the sensor data from the ADC buffer and calculates the average values.
 *
 * This function reads the sensor data from the ADC buffer and calculates the average
 * values for the voltage and current sensors based on the collected samples. It processes
 * the ADC data to obtain the sensor readings in the desired units (e.g., voltage in V,
 * current in A) and returns the calculated values for further processing or display.
 * 
 * The function should be called after the ADC samples have been collected and stored in
 * the buffer. It processes the data to obtain the sensor readings and returns the calculated
 * values for use in the application logic.
 * 
 * @return An array of floating-point values containing the average sensor readings.
 */

float get_sensor_values(uint8_t channel);



/**
 * @brief average of the stored phase shift values
 * This function iterates through the phase shift buffer and calculates the average of these values.
 * 
 * @return An average of the values stored in the buffer.
 */
float average_phase_shift(void);


/**
 * @brief Led brightness
 * This function adjusts the LED brightness based on critical consumption values, using the PWM module for this purpose.
 */
void adjust_led_intensity(void);
