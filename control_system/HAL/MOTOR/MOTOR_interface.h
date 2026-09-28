#ifndef MOTOR_INTERFACE_H
#define MOTOR_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/* Direction Macros */
#define MOTOR_u8_CW    0   /* Clockwise */
#define MOTOR_u8_CCW   1   /* Counter-Clockwise */

/*
   Initializes Timer1 for PWM generation.
   Note: DIO pin directions are already configured in main.c
*/
void MOTOR_voidInit(void);

/*
   Set Motor Direction
   Options: MOTOR_u8_CW or MOTOR_u8_CCW
*/
void MOTOR_voidSetDirection(u8 Copy_u8Direction);

/*
   Set Motor Speed
   Input: 0 (Stopped) to 255 (Max Speed)
*/
void MOTOR_voidSetSpeed(u8 Copy_u8Speed);

/* Immediately stops the motor */
void MOTOR_voidStop(void);

#endif
