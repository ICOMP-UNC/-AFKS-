#include "lcd.h"
#include "sensor_acquisition.h"

// Function prototypes
float get_sensor_values(void);

// Definición de variables globales
uint32_t voltaje = 220;  // en V
uint32_t corriente = 53;  // en A
uint32_t factor_potencia = 5; // Adimensional [fp=cos(ϕ)]
uint32_t potencia;

float avg_voltage = 0;  // Promedio de voltaje
uint32_t avg_current = 0;  // Promedio de corriente
static volatile uint16_t potBuff[ADC_SAMPLE_COUNT]; /** Buffer to store ADC readings for the potentiometer */



void update_display() {
    
 char line[17];
    potencia = (voltaje * corriente) / 100; // Calcular potencia en W
    avg_voltage = get_sensor_values();  // Obtener el valor promedio del voltaje
    // Mostrar voltaje en la primera línea
    snprintf(line, sizeof(line), "Voltaje: %u V", avg_voltage);
    lcd_set_cursor(0, 0);
    lcd_print_string(line);

    // Mostrar corriente en la segunda línea
    snprintf(line, sizeof(line), "Corriente: %u A", avg_current);
    lcd_set_cursor(1, 0);
    lcd_print_string(line);

    // Mostrar potencia en la tercera línea
    snprintf(line, sizeof(line), "Potencia: %u W", potencia);
    lcd_set_cursor(2, 0);
    lcd_print_string(line);

    // Mostrar factor de potencia en la cuarta línea
    snprintf(line, sizeof(line), "Fp: %u", 0);
    lcd_set_cursor(3, 0);
    lcd_print_string(line);


}



float get_sensor_values(void)
{
    uint32_t accumulator = 0;  // Acumulador para los valores del buffer

    // Sumar todos los valores en el buffer
    for (uint8_t i = 0; i < ADC_SAMPLE_COUNT; i++)
    {
        accumulator += potBuff[i];
    }

    // Calcular el promedio
    float average_value = (float)accumulator / ADC_SAMPLE_COUNT;

    // Convertir a porcentaje usando la constante adecuada
    // Supongamos que el ADC es de 12 bits (rango de 0 a 4095)
    return (average_value / 4095.0f) * 100.0f;
}
