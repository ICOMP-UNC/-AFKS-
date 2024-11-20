
#include "timer_exti.h"

static volatile uint32_t phase_shift_buffer[N_PHASE_SHIFT]; /**< Buffer para almacenar los desplazamientos de fase. */

static volatile uint32_t  counter=0;

static volatile uint32_t phase_shift=0;

void system_init(void)
{
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_TIM1);
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_AFIO);
}
void gpio_setup(void)
{
   
    gpio_set_mode(EXTI_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, EXTI_PIN0|EXTI_PIN1);

    // Configurar PA10 como salida alternativa para PWM
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO10);
}



void TMR_setup_PF(void)
{
    /*timer2 config*/
    rcc_periph_reset_pulse(RST_TIM2);
    timer_set_prescaler(TIM2,PREESCALER_TM2-1);     //cuenta cada 0.1 mseg
    timer_direction_up(TIM2);
}


void TMR_setup_pwm(void) {
    // Configurar Timer 1 para PWM en el canal 3
    timer_disable_counter(TIM1);
    timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM1, PREESCALER_TM1 - 1); // 1 MHz timer clock
    timer_set_period(TIM1, PERIOD_TM1);      // 1 kHz PWM frecuencia
    timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
    timer_enable_oc_output(TIM1, TIM_OC3);
    timer_enable_break_main_output(TIM1);
    timer_enable_counter(TIM1);
}




void EXTI_setup_PF(void)
{
    nvic_enable_irq(NVIC_EXTI2_IRQ);
    nvic_enable_irq(NVIC_EXTI3_IRQ);
    exti_select_source(EXTI2|EXTI3, EXTI_PORT);
    exti_set_trigger(EXTI2|EXTI3, EXTI_TRIGGER_RISING);   //interrumpe por flanco asc
    exti_enable_request(EXTI2|EXTI3);
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
        phase_shift = average_phase_shift();
        counter=0;
    }
    timer_disable_counter(TIM2);

}


float average_phase_shift(void)
{

    for(int i=0; i<N_PHASE_SHIFT; i++)
    {
        phase_shift+=phase_shift_buffer[i];
    }
   return  (phase_shift / N_PHASE_SHIFT) ;
}