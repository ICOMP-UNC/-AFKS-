/**
 * @file timer_exti.h
 * @brief Phase Shift determination
 * This file provides functions and definitions for initializing Port A pins 
 * as external interrupt inputs, configuring Timer 2, and determining the phase 
 * shift between two signals. 
 */

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>

/**
 * @brief define the Extern Interruptions Port
 */
#define EXTI_PORT   GPIOA

/**
 * @brief define exti pin 
 */
#define EXTI_PIN0   GPIO2

/**
 * @brief define exti pin
 */
#define EXTI_PIN1   GPIO3

/**
 * @brief define the square root of two
 */
#define SQRT_2 1.4142

/**
 * @brief define the buffer shape shift size
 */
#define N_PHASE_SHIFT 10

/**
 * @brief define half period in [us] of a 50 [Hz] signal
 */
#define HALF_PERIOD 100

#define MS_CONVERSION 10 // conversion de us a mS

#define PREESCALER_TM1 72

#define PREESCALER_TM2 7200

#define PERIOD_TM1 1000

//uint32_t phase_shift_buffer[N_PHASE_SHIFT]={0};


static volatile uint32_t phase_shift_buffer[N_PHASE_SHIFT]; /**< Buffer para almacenar los desplazamientos de fase. */

static volatile uint32_t  counter=0;


/**
 * @brief Enable the clock for the involved modules.
 * This function enables the clock for GPIO, EXTI, and TMR2
 */
void system_init(void);

/**
 * @brief in and port configuration
 * This function configures the pins for EXTI as inputs with pull-down resistors
 */
void gpio_setup(void);

/**
 * @brief Timer2 configuration
 * This function sets the Timer 2 frequency to 10 [kHz], meaning 
 * the counter will increment by one every 100 [us] and it configures 
 * the timer for continuous counting without generating interrupts
 */
void TMR_setup_PF(void);

/**
 * @brief EXTI configuration
 * This function configures pins A2 and A3 to generate external 
 * interrupts on rising edge.
 */
void EXTI_setup_PF(void);  





/**
 * @brief Configures Timer 1 for PWM generation.
 * 
 * Initializes Timer 1 to generate a PWM signal with a frequency of 1 kHz on channel 3.
 */

void TMR_setup_pwm(void);
