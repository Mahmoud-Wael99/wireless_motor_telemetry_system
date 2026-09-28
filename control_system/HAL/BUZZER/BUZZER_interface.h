#ifndef BUZZER_INTERFACE_H
#define BUZZER_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/* Turns the buzzer on (High) */
void BUZZER_voidOn(void);

/* Turns the buzzer off (Low) */
void BUZZER_voidOff(void);

/* Toggles the current state of the buzzer */
void BUZZER_voidToggle(void);

#endif
