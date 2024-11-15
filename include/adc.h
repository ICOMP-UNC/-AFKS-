#ifndef ADC_H
#define ADC_H

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void ADC_Init(void);
uint32_t ADC_Read(void);

#endif // ADC_H