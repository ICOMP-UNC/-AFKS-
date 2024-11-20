/**
 * @file timer_exti.c
 * @brief Implementation of timer and external interrupt (EXTI) configurations and functions.
 * 
 * This file provides the initialization and setup of timers and EXTI for phase shift measurement,
 * including PWM generation, EXTI configuration, and phase shift calculation.
 * 
 * @note This file should be used in conjunction with its header file `timer_exti.h`.
 */

#include "timer_exti.h"

/** @brief Buffer to store phase shift values for averaging. */
static volatile uint32_t phase_shift_buffer[N_PHASE_SHIFT];

/** @brief Counter to track the number of stored phase shifts. */
static volatile uint32_t counter = 0;

/** @brief Calculated phase shift average. */
static volatile uint32_t phase_shift = 0;

/**
 * @brief Initializes the system clock and enables peripheral clocks.
 * 
 * Configures the system clock using an external 8 MHz crystal and enables
 * the clocks for GPIO, TIM1, TIM2, and alternate function IO.
 */
void system_init(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_TIM1);
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_AFIO);
}

/**
 * @brief Configures GPIO pins for input and output functionality.
 * 
 * - Configures EXTI_PORT pins as input with pull-up/down resistors for interrupts.
 * - Configures GPIOA pin 10 as an alternate function output for PWM.
 */
void gpio_setup(void) {
    gpio_set_mode(EXTI_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, EXTI_PIN0 | EXTI_PIN1);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO10);
}

/**
 * @brief Configures Timer 2 for phase shift measurement.
 * 
 * Timer 2 is set to count upwards with a prescaler for a resolution of 0.1 ms.
 */
void TMR_setup_PF(void) {
    rcc_periph_reset_pulse(RST_TIM2);
    timer_set_prescaler(TIM2, PREESCALER_TM2 - 1);
    timer_direction_up(TIM2);
}

/**
 * @brief Configures Timer 1 for PWM generation on channel 3.
 * 
 * - Timer 1 is set to generate a PWM signal at 1 kHz.
 * - Configures Timer 1 for PWM mode 1 on channel 3.
 */
void TMR_setup_pwm(void) {
    timer_disable_counter(TIM1);
    timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM1, PREESCALER_TM1 - 1);
    timer_set_period(TIM1, PERIOD_TM1);
    timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
    timer_enable_oc_output(TIM1, TIM_OC3);
    timer_enable_break_main_output(TIM1);
    timer_enable_counter(TIM1);
}

/**
 * @brief Configures EXTI lines for phase shift measurement.
 * 
 * - Enables interrupts for EXTI2 and EXTI3.
 * - Configures rising edge trigger for EXTI2 and EXTI3.
 */
void EXTI_setup_PF(void) {
    nvic_enable_irq(NVIC_EXTI2_IRQ);
    nvic_enable_irq(NVIC_EXTI3_IRQ);
    exti_select_source(EXTI2 | EXTI3, EXTI_PORT);
    exti_set_trigger(EXTI2 | EXTI3, EXTI_TRIGGER_RISING);
    exti_enable_request(EXTI2 | EXTI3);
}

/**
 * @brief EXTI2 interrupt service routine (ISR).
 * 
 * Resets the EXTI2 interrupt request, resets Timer 2 counter to 0, and starts Timer 2.
 */
void exti2_isr(void) {
    exti_reset_request(EXTI2);
    timer_set_counter(TIM2, 0);
    timer_enable_counter(TIM2);
}

/**
 * @brief EXTI3 interrupt service routine (ISR).
 * 
 * - Resets the EXTI3 interrupt request.
 * - Stores the Timer 2 counter value in the phase shift buffer.
 * - Calculates the average phase shift when the buffer is full.
 */
void exti3_isr(void) {
    exti_reset_request(EXTI3);
    if (counter < N_PHASE_SHIFT) {
        phase_shift_buffer[counter] = timer_get_counter(TIM2);
        counter++;
    } else if (counter == N_PHASE_SHIFT) {
        phase_shift = average_phase_shift();
        counter = 0;
    }
    timer_disable_counter(TIM2);
}

/**
 * @brief Calculates the average phase shift from the buffer.
 * 
 * @return The average phase shift as a floating-point value.
 */
float average_phase_shift(void) {
    for (int i = 0; i < N_PHASE_SHIFT; i++) {
        phase_shift += phase_shift_buffer[i];
    }
    return (phase_shift / N_PHASE_SHIFT);
}
