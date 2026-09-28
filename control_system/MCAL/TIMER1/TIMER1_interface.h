#ifndef TIMER1_INTERFACE_H
#define TIMER1_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

void TIMER1_voidInit(void);

/* Pass a value from 0 (0% speed) to 255 (100% speed) */
void TIMER1_voidSetDutyCycle(u8 Copy_u8DutyCycle);

#endif
