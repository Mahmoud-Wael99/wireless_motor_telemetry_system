#include <math.h>
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/ADC/ADC_interface.h"

#include "SENSORS_interface.h"
#include "SENSORS_config.h"

void SENSORS_voidInit(void)
{
    /*
       Hardware initialization is handled by ADC_voidInit() in main.c,
       but this acts as an abstraction placeholder for higher-level logic.
    */
}

u16 SENSORS_u16GetMillivolts(void)
{
    u16 Local_u16ADCValue = ADC_u16GetChannelReading(SENSORS_u8_VOLTAGE_CHANNEL);

    /*
       1. Convert 10-bit ADC value to pin voltage (Vout)
       2. Reconstruct original voltage (Vin) using the divider ratio
    */
    f32 Local_f32Vout = ((f32)Local_u16ADCValue * 5.0) / 1024.0;
    f32 Local_f32Vin = Local_f32Vout / (SENSORS_f32_R2 / (SENSORS_f32_R1 + SENSORS_f32_R2));

    /* Convert to millivolts and cast to u16 */
    return (u16)(Local_f32Vin * 1000.0);
}

u8 SENSORS_u8GetTemperature(void)
{
    u16 Local_u16ADCValue = ADC_u16GetChannelReading(SENSORS_u8_NTC_CHANNEL);

    /* Prevent division by zero if pin is shorted to ground or VCC */
    if (Local_u16ADCValue == 0 || Local_u16ADCValue >= 1023)
    {
        return SENSORS_u8_TEMP_ERROR;
    }

    /* Calculate actual NTC resistance based on the divider */
    f32 Local_f32Resistance = SENSORS_u16_SERIES_RESISTOR / ((1024.0 / (f32)Local_u16ADCValue) - 1.0);

    /* Apply Steinhart-Hart equation */
    f32 Local_f32Temp;
    Local_f32Temp = Local_f32Resistance / SENSORS_u16_NOMINAL_RESIST;
    Local_f32Temp = log(Local_f32Temp);
    Local_f32Temp /= SENSORS_u16_B_COEFFICIENT;
    Local_f32Temp += 1.0 / (SENSORS_u8_NOMINAL_TEMP + 273.15);
    Local_f32Temp = 1.0 / Local_f32Temp;
    Local_f32Temp -= 273.15; /* Convert Kelvin to Celsius */

    /* Casting a negative float to u8 is undefined: clamp first */
    if (Local_f32Temp < 0.0)
    {
        Local_f32Temp = 0.0;
    }
    else if (Local_f32Temp > 254.0)
    {
        Local_f32Temp = 254.0;
    }

    return (u8)Local_f32Temp;
}
