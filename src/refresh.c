#include "refresh.h"



int average_phase_shift(void)
{
    uint32_t phase_shift=0;

    for(int i=0; i<N_PHASE_SHIFT; i++)
    {
        phase_shift+=phase_shift_buffer[i];
    }
     return (phase_shift/N_PHASE_SHIFT) ;
}



float get_sensor_values(uint8_t channel) {
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
    float power = get_sensor_values(0) * get_sensor_values(1);
 
     // Show ADC0 value on the first line
    snprintf(line, sizeof(line), "Volt A0: %u V", (unsigned int)get_sensor_values(0));
    lcd_set_cursor(0, 0);
    lcd_print_string(line);


    // Show ADC1 value on the second line
    snprintf(line, sizeof(line), "Current: %u A", (unsigned int) get_sensor_values(1));
    lcd_set_cursor(1, 0);
    lcd_print_string(line);


    snprintf(line, sizeof(line), "Power : %u KW", (unsigned int) power);
    lcd_set_cursor(2, 0);
    lcd_print_string(line);

    
    snprintf(line, sizeof(line), "Phase : %u mS", (unsigned int) average_phase_shift/MS_CONVERSION);
    lcd_set_cursor(3, 0);
    lcd_print_string(line);
    
}

void set_pwm_duty_cycle(uint16_t duty) {
    if (duty > 1000) duty = 1000; // Limitar el duty cycle
    timer_set_oc_value(TIM1, TIM_OC3, duty); // Configurar el valor en TIM1_CH3
}

void adjust_led_intensity(void) {
    if (pot_get_value(1) < CURRENT_MIN) {
        set_pwm_duty_cycle(0); // LED apagado
    } else if (pot_get_value(1) <= CURRENT_MAX) {
        uint16_t duty = (uint16_t)((pot_get_value(1) - CURRENT_MIN) * 1000 / (CURRENT_MAX - CURRENT_MIN));
        set_pwm_duty_cycle(duty); // Ajustar intensidad del LED
    } else {
        set_pwm_duty_cycle(1000); // Máxima intensidad
    }
}


    