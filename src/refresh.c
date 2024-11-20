#include "refresh.h"
#include "adc_dma.h"
#include "lcd.h"
#include "stdio.h"
#include "timer_exti.h"


int average_phase_shift(void)
{
    uint32_t phase_shift=0;

    for(int i=0; i<N_PHASE_SHIFT; i++)
    {
        phase_shift+=phase_shift_buffer[i];
    }
     return (phase_shift/N_PHASE_SHIFT) ;
}



float pot_get_value(uint8_t channel) {
    uint32_t ac = 0;

    // Suma los valores en el buffer dependiendo del canal seleccionado
    for (uint8_t i = 0; i < ADC_SAMPLE_COUNT; i++) {
        ac += ADC_BUFFER[channel + i * ADC_CHANNEL_COUNT];  // Acceso intercalado por canal
    }

    // Calcula el promedio
    float average = (float)ac / (float)ADC_SAMPLE_COUNT;

    // Linealiza según el canal
    if (channel == 0) {
        // Canal de voltaje: 0 bits corresponde a 0V y 4096 bits a 230V
        return (230.0 / 4096.0) * average;
    } else if (channel == 1) {
        // Canal de corriente: 0 bits corresponde a 0A y 4096 bits a 10A
        return (10.0 / 4096.0) * average;
    }

    return 0; // En caso de un canal inválido
}


void update_display(void) {
      char line[17];
    float power = pot_get_value(0) * pot_get_value(1);
 
     // Show ADC0 value on the first line
    snprintf(line, sizeof(line), "Volt A0: %u V", (unsigned int)pot_get_value(0));
    lcd_set_cursor(0, 0);
    lcd_print_string(line);


    // Show ADC1 value on the second line
    snprintf(line, sizeof(line), "Current: %u A", (unsigned int) pot_get_value(1));
    lcd_set_cursor(1, 0);
    lcd_print_string(line);


    snprintf(line, sizeof(line), "Power : %u KW", (unsigned int) power);
    lcd_set_cursor(2, 0);
    lcd_print_string(line);

    
    snprintf(line, sizeof(line), "Phase : %u mS", (unsigned int) average_phase_shift/MS_CONVERSION);
    lcd_set_cursor(3, 0);
    lcd_print_string(line);
    
}