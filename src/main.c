#include "refresh.h"


int main(void) {
    system_init();
    setup_gpio();
    config_adc_dma();
    TMR_setup_PF();
    TMR_setup_pwm();
    EXTI_setup_PF();
    lcd_init();

    while (TRUE) {
   
        update_values();
    }
    return 0;
}

