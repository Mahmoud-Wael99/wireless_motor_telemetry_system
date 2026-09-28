#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/DIO/DIO_interface.h"

#include "BUZZER_interface.h"
#include "BUZZER_config.h"

void BUZZER_voidOn(void)
{
    DIO_voidSetPinValue(BUZZER_u8_PORT, BUZZER_u8_PIN, DIO_u8_HIGH);
}

void BUZZER_voidOff(void)
{
    DIO_voidSetPinValue(BUZZER_u8_PORT, BUZZER_u8_PIN, DIO_u8_LOW);
}

void BUZZER_voidToggle(void)
{
    /* Read the current state and invert it */
    u8 Local_u8CurrentState = DIO_u8GetPinValue(BUZZER_u8_PORT, BUZZER_u8_PIN);

    if (Local_u8CurrentState == DIO_u8_HIGH)
    {
        DIO_voidSetPinValue(BUZZER_u8_PORT, BUZZER_u8_PIN, DIO_u8_LOW);
    }
    else
    {
        DIO_voidSetPinValue(BUZZER_u8_PORT, BUZZER_u8_PIN, DIO_u8_HIGH);
    }
}
