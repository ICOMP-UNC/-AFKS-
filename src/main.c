#include "refresh.h"






int main(void) {
  
    config_gpio();
    config_adc_dma();
    lcd_init();

    while (1) {
   
        update_display();
    }
    return 0;
}

