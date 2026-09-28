#ifndef ADC_INTERFACE_H
#define ADC_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

void ADC_voidInit(void);

/* Pass 2 to read the NTC on PA2, or 4 to read the Voltage Sensor on PA4 */
u16 ADC_u16GetChannelReading(u8 Copy_u8Channel);

#endif
