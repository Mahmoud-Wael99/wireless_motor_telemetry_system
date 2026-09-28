#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "TIMER1_interface.h"
#include "TIMER1_private.h"

void TIMER1_voidInit(void)
{
    /* 1. Set Timer1 to Fast PWM 8-bit Mode (Mode 5) */
    /* WGM13=0, WGM12=1, WGM11=0, WGM10=1 */
    SET_BIT(TIMER1_u8_TCCR1A_REG, 0); // WGM10
    CLR_BIT(TIMER1_u8_TCCR1A_REG, 1); // WGM11
    SET_BIT(TIMER1_u8_TCCR1B_REG, 3); // WGM12
    CLR_BIT(TIMER1_u8_TCCR1B_REG, 4); // WGM13

    /* 2. Select Non-Inverting PWM on OC1A (PD5) */
    /* Clear OC1A on compare match, set OC1A at BOTTOM */
    CLR_BIT(TIMER1_u8_TCCR1A_REG, 6); // COM1A0
    SET_BIT(TIMER1_u8_TCCR1A_REG, 7); // COM1A1

    /* 3. Set Prescaler to 64 -> Starts the Timer */
    /* CS12=0, CS11=1, CS10=1 */
    SET_BIT(TIMER1_u8_TCCR1B_REG, 0); // CS10
    SET_BIT(TIMER1_u8_TCCR1B_REG, 1); // CS11
    CLR_BIT(TIMER1_u8_TCCR1B_REG, 2); // CS12

    /* Initialize with 0% duty cycle (Motor stopped) */
    TIMER1_u16_OCR1A_REG = 0;
}

void TIMER1_voidSetDutyCycle(u8 Copy_u8DutyCycle)
{
    /* Since we are in 8-bit Fast PWM mode, OCR1A takes a value between 0 and 255 */
    TIMER1_u16_OCR1A_REG = Copy_u8DutyCycle;
}
