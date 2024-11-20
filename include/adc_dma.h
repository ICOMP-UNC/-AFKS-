/*
    @file sensor_acquisition.h
    @brief Sensor acquisition functions

    This file contains the prototypes and definitions used to acquire data from the sensors connected to ADC channels.

 */

#include "libopencm3/stm32/adc.h"
#include "libopencm3/stm32/dma.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/rcc.h"
#include "lcd.h"
#include "stdio.h"

/** @brief Number of ADC samples to collect for each channel in DMA. */
#define ADC_SAMPLE_COUNT 10

/** @brief Number of channels to be sampled by the ADC. */
#define ADC_CHANNEL_COUNT 2

/** @brief ADC channel assigned to voltage measurement (GPIOA pin A0). */
#define ADC_VOLTAGE_CHANNEL ADC_CHANNEL0

/** @brief ADC channel assigned to current measurement (GPIOA pin A1). */
#define ADC_CURRENT_CHANNEL ADC_CHANNEL1

/** @brief GPIO port for ADC input pins. */
#define ADC_GPIO_PORT GPIOA

/** @brief GPIO pin for voltage input connected to ADC channel 0. */
#define ADC_VOLTAGE_PIN GPIO0

/** @brief GPIO pin for current input connected to ADC channel 1. */
#define ADC_CURRENT_PIN GPIO1


/** @brief ADC sample time configuration. */
#define SAMPLE_TIME_CYCLES ADC_SMPR_SMP_28DOT5CYC


#define ADC_BUFFER_SIZE (ADC_SAMPLE_COUNT * ADC_CHANNEL_COUNT)  // Tamaño del buffer para almacenar las muestras ADC


static volatile uint16_t ADC_BUFFER[ADC_BUFFER_SIZE];  // Buffer compartido para ambos canales ADC

/**
 * @brief Configures ADC pins and initializes DMA for sensor data acquisition.
 *
 * This function performs the necessary configuration to set up the ADC to read values
 * from the sensors connected to the specified ADC channels. It configures the GPIO pins
 * for ADC input, enables the ADC and DMA peripherals, and sets the required sample times
 * for accurate sensor readings.
 * 
 * It initializes the DMA to transfer the sensor data efficiently and stores the samples
 * in a buffer for further processing. The ADC channels, DMA settings, and sampling times
 * are configured according to the application requirements (e.g., voltage and current sensors).
 * 
 * The function should be called once at the start of the program to initialize the hardware
 * before any ADC readings are taken.
 */
void config_adc_dma(void);



