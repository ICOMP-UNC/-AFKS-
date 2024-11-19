#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
#include "math.h"

#define EXTI_PORT   GPIOA
#define EXTI_PIN0   GPIO2
#define EXTI_PIN1   GPIO3

#define PI 3.1415
#define N_PHASE_SHIFT 10
#define HALF_PERIOD 10

uint32_t phase_shift_buffer[N_PHASE_SHIFT]={0};
uint32_t phase_shift=0;
uint32_t power_factor=0;
uint8_t  counter=0;


void SystemInit_PF(void);
void GPIO_setup_PF(void);
void TMR_setup_PF(void);
void EXTI_setup_PF(void);  

void clear_phase_shift(void);
void average_phase_shift(void);
void calculate_power_factor(void);

