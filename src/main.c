/**
 * @file main.c
 * @brief Main program for system initialization and LCD control.
 *
 * This program initializes the system, sets up peripherals like GPIO, ADC, DMA, timers, and interrupts,
 * and continuously updates values to interact with the LCD or other peripherals.
 */

#include "refresh.h"

/**
 * @brief Main entry point of the program.
 *
 * This function performs the initial setup of the system, including initializing peripherals
 * such as GPIO, ADC with DMA, timers, external interrupts, and the LCD. Once initialized, it enters
 * an infinite loop to repeatedly update values.
 * 
 * @return Always returns 0 (success).
 */
int main(void) {
    // System and peripheral initialization
    system_init();        /* Initialize system clock and basic configuration. */
    gpio_setup();         /* Configure GPIO pins for input/output as required. */
    config_adc_dma();     /* Set up ADC with DMA for continuous data acquisition. */
    TMR_setup_PF();       /* Configure periodic timer for regular updates. */
    TMR_setup_pwm();      /* Configure a timer for PWM signal generation. */
    EXTI_setup_PF();      /* Set up external interrupts for specific GPIO pins. */
    lcd_init();           /* Initialize the LCD display for interaction. */

    // Main loop
    while (TRUE) {
        update_values();  /**< Continuously update values based on inputs or system state. */
    }

    return 0; // Should never be reached
}
