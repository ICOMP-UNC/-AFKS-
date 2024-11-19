#include "PowerFactor.h"

void SystemInit_PF(void)
{
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_TIM3);
    rcc_periph_clock_enable(RCC_AFIO);
}
void GPIO_setup_PF(void)
{
    gpio_set_mode(EXTI_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, EXTI_PIN0|EXTI_PIN1);
}

void TMR_setup_PF(void)
{
    /*timer2 config*/
    rcc_periph_reset_pulse(RST_TIM2);
    timer_set_prescaler(TIM2,7200-1);     //cuenta cada 1 mseg
    timer_direction_up(TIM2);

    /*timer3 config*/
    rcc_periph_reset_pulse(RST_TIM3);
    timer_disable_counter(TIM3);
    timer_set_counter(TIM3, 0);
    timer_enable_counter(TIM3);
    timer_set_prescaler(TIM3,72000-1);      //cuenta cada 1 mseg
    timer_set_period(TIM3, 2100);           // aca ver cada cuanto tiempo interrumpe, primeramente un poco mas de 10 periodos
    TIM3_DIER|=TIM_DIER_UIE;                //habilito interrupcion por desborde
    nvic_enable_irq(NVIC_TIM3_IRQ);         //habilito IRQ en NVIC
}

void EXTI_setup_PF(void)
{
    nvic_enable_irq(NVIC_EXTI2_IRQ);
    nvic_enable_irq(NVIC_EXTI3_IRQ);
    exti_select_source(EXTI2|EXTI3, EXTI_PORT);
    exti_set_trigger(EXTI2|EXTI3, EXTI_TRIGGER_RISING);   //interrumpe por flanco asc
    exti_enable_request(EXTI2|EXTI3);
}

void clear_phase_shift(void)
{
    for(int i=0; i<N_PHASE_SHIFT; i++)
    {
        phase_shift_buffer[i]=0;
    }
}

void average_phase_shift(void)
{
    for(int i=0; i<N_PHASE_SHIFT; i++)
    {
        phase_shift+=phase_shift_buffer[i];
    }
    phase_shift/=N_PHASE_SHIFT;
}

void calculate_power_factor(void)
{
    power_factor=cos((phase_shift*PI/HALF_PERIOD));
}

void exti2_isr(void)
{
    exti_reset_request(EXTI2);
    timer_set_counter(TIM2, 0);
    timer_enable_counter(TIM2);
}

void exti3_isr(void)
{
    exti_reset_request(EXTI3);
    if(counter<N_PHASE_SHIFT)
    {
        phase_shift_buffer[counter]=timer_get_counter(TIM2);
        counter++;
    }
    else if(counter==N_PHASE_SHIFT)
    {
        counter=0;
    }
    timer_disable_counter(TIM2);
}

void tim3_isr(void)
{
    TIM3_SR&=~TIM_SR_UIF;       //limpio flag
    average_phase_shift();
    calculate_power_factor();
    clear_phase_shift();
}