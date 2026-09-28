#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/DIO/DIO_interface.h"
#include "../../MCAL/TIMER1/TIMER1_interface.h"

#include "MOTOR_interface.h"
#include "MOTOR_config.h"

void MOTOR_voidInit(void)
{
    /* Initialize Timer1 to generate the PWM signal for the ENA pin */
    TIMER1_voidInit();
}

void MOTOR_voidSetDirection(u8 Copy_u8Direction)
{
    if (Copy_u8Direction == MOTOR_u8_CW)
    {
        /* IN1 = HIGH, IN2 = LOW */
        DIO_voidSetPinValue(MOTOR_u8_DIR_PORT, MOTOR_u8_DIR1_PIN, DIO_u8_HIGH);
        DIO_voidSetPinValue(MOTOR_u8_DIR_PORT, MOTOR_u8_DIR2_PIN, DIO_u8_LOW);
    }
    else if (Copy_u8Direction == MOTOR_u8_CCW)
    {
        /* IN1 = LOW, IN2 = HIGH */
        DIO_voidSetPinValue(MOTOR_u8_DIR_PORT, MOTOR_u8_DIR1_PIN, DIO_u8_LOW);
        DIO_voidSetPinValue(MOTOR_u8_DIR_PORT, MOTOR_u8_DIR2_PIN, DIO_u8_HIGH);
    }
}

void MOTOR_voidSetSpeed(u8 Copy_u8Speed)
{
    /* Pass the speed value (0-255) to the Timer1 PWM generator */
    TIMER1_voidSetDutyCycle(Copy_u8Speed);
}

void MOTOR_voidStop(void)
{
    /* Set speed to 0 and pull both direction pins low to brake */
    TIMER1_voidSetDutyCycle(0);
    DIO_voidSetPinValue(MOTOR_u8_DIR_PORT, MOTOR_u8_DIR1_PIN, DIO_u8_LOW);
    DIO_voidSetPinValue(MOTOR_u8_DIR_PORT, MOTOR_u8_DIR2_PIN, DIO_u8_LOW);
}
