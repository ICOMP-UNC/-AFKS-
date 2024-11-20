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

/**
 * @brief Configures the system clock.
 * 
 * Sets the system clock to 72 MHz using the external high-speed oscillator (HSE).
 */

void setup_gpio(void);



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