#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <math.h>

#define CAPTURE_PORT    GPIOA
#define CAPTURE_PIN1    GPIO0 
#define CAPTURE_PIN2    GPIO1

uint8_t PrimerCap[10]={0,0,0,0,0,0,0,0,0,0};
uint8_t SegundoCap[10]={0,0,0,0,0,0,0,0,0,0};
uint8_t Contador1=0;
uint8_t Contador2=0;
uint8_t Promedio1=0;
uint8_t Promedio2=0;

float fdp=0;

/*prototype functions*/
void cfg_port(void);
void cfg_TMR2(void);
void cfg_TMR3(void);
void clear_captures_array(void);
void average_times(void);
void fdp_calculate(void);

void cfg_port(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);    
    gpio_set_mode(CAPTURE_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, CAPTURE_PIN1);
    gpio_set_mode(CAPTURE_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, CAPTURE_PIN2);
}

void cfg_TMR3(void)
{
    rcc_periph_clock_enable(RCC_TIM3);
    rcc_periph_reset_pulse(RST_TIM3);
    timer_set_prescaler(TIM3, 72000-1); /*TMR3 counter cada 1ms*/
    timer_set_period(TIM3, 210);        /*interrupt each 210ms */
    timer_direction_up(TIM3);
    nvic_enable_irq(NVIC_TIM3_IRQ);
}

void cfg_TMR2(void)
{   
    /*timers clocks*/
    rcc_periph_clock_enable(RCC_TIM2);

    /*reset timer*/
    rcc_periph_reset_pulse(RST_TIM2);
   

    /*timer config prescaler*/
    timer_set_prescaler(TIM2, 72000-1); /*TMR2 counter cada 1ms*/       

    /*capture config*/
    timer_ic_set_input(TIM2, TIM_IC1, TIM_IC_IN_TI1);   /*TI1 capture channel1*/
    timer_ic_set_input(TIM2, TIM_IC2, TIM_IC_IN_TI2);   /*TI2 capture channel2*/

    /*timer capture filter*/
    timer_ic_set_filter(TIM2, TIM_IC1, TIM_IC_OFF);
    timer_ic_set_filter(TIM2, TIM_IC2, TIM_IC_OFF);

    /*Polarity*/
    timer_ic_set_polarity(TIM2, TIM_IC1, TIM_IC_RISING);
    timer_ic_set_polarity(TIM2, TIM_IC2, TIM_IC_RISING);

    /*enable capture channel1 and channel2*/
    timer_ic_enable(TIM2, TIM_IC1|TIM_IC2);
    
    /*enable timer2 interruption by captures and timer 3 interruption*/
    nvic_enable_irq(NVIC_TIM2_IRQ);
    
    timer_enable_irq(TIM2,TIM_DIER_CC1IE);
    timer_enable_irq(TIM2, TIM_DIER_CC2IE); 

}

/*IRQ functions*/
void tim2_isr(void)
{
    if(timer_get_flag(TIM2, TIM_SR_CC1IF))  /*interrumpio capture1?*/
    {
        if(Contador1<10)
        {   
            PrimerCap[Contador1]=timer_get_counter(TIM2);
            Contador1++;
        }
        else if (Contador1==10)
        {
            Contador1=0;
        }
        timer_clear_flag(TIM2, TIM_SR_CC1IF);
    }

    else if(timer_get_flag(TIM2, TIM_SR_CC2IF)) //interrumpio capture2?
    {
        if(Contador2<10)
        {
            SegundoCap[Contador2]=timer_get_counter(TIM2);
            Contador2++;
            timer_set_counter(TIM2, 0);         /*reinicio el contador*/
        }
        else if(Contador2==10)
        {
            Contador2=0;
            timer_set_counter(TIM2, 0);  
        }
        timer_clear_flag(TIM2, TIM_SR_CC2IF);
    }  
}

/*cada cierto tiempo (al menos 210ms (mas de10 perdiodos de señal)) interrumpir para hacer promedio
  y limpiar los valores de los arrays*/

void tim3_isr(void)
{   
    nvic_disable_irq(NVIC_TIM2_IRQ);
    average_times();
    fdp_calculate();
    clear_capture_arrays();
    timer_set_counter(TIM2, 0);
    nvic_enable_irq(NVIC_TIM2_IRQ);
}


/*funciones auxiliares*/
void clear_capture_arrays(void)
{
    for (uint8_t i=0; i<10, i++)
    {
        PrimerCap[i]=0;
        SegundoCap[i]=0;
    }
}

void average_times(void)
{
    for (uint8_t i=0; i<10; i++)
    {
        Promedio1+=PrimerCap[i]; 
        Promedio2+=SegundoCap[i];
    }
    Promedio1/=10;
    Promedio2/=10;
}

void fdp_calculate(void)
{
    fdp=cos(360*(Promedio1-Promedio2)/20);
}
