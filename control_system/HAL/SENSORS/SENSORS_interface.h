#ifndef SENSORS_INTERFACE_H
#define SENSORS_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/* Initializes the Sensors layer (optional wrapper) */
void SENSORS_voidInit(void);

/* Returns the main 12V power supply voltage in millivolts */
u16 SENSORS_u16GetMillivolts(void);

/* Value returned by SENSORS_u8GetTemperature() when the NTC is open/shorted
   (a real reading of 0 C is therefore no longer confused with an error) */
#define SENSORS_u8_TEMP_ERROR   255

/* Returns the motor temperature in degrees Celsius.
   Readings below 0 C are clamped to 0, above 254 C to 254.
   Returns SENSORS_u8_TEMP_ERROR if the NTC pin is shorted or disconnected. */
u8 SENSORS_u8GetTemperature(void);

#endif
