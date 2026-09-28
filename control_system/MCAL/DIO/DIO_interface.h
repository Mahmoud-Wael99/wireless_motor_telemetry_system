// File Guard
#ifndef DIO_INTERFACE_H
#define DIO_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

// Register Macros
#define DIO_u8_PORTA	0
#define DIO_u8_PORTB	1
#define DIO_u8_PORTC	2
#define DIO_u8_PORTD	3

// Pins Macros
#define DIO_u8_PIN0		0
#define DIO_u8_PIN1		1
#define DIO_u8_PIN2		2
#define DIO_u8_PIN3		3
#define DIO_u8_PIN4		4
#define DIO_u8_PIN5		5
#define DIO_u8_PIN6		6
#define DIO_u8_PIN7		7

// Other Macros
#define DIO_u8_INITIAL_OUTPUT	1
#define DIO_u8_INITIAL_INPUT	0

#define PORT_OUTPUT		0xFF

#define DIO_u8_LOW	0
#define DIO_u8_HIGH	1

void DIO_voidSetPinValue(u8 Copy_u8PortID, u8 Copy_u8PinID, u8 Copy_u8Value);

void DIO_voidSetPinDirection(u8 Copy_u8PortID, u8 Copy_u8PinID, u8 Copy_u8Direction);

u8 DIO_u8GetPinValue(u8 Copy_u8PortID, u8 Copy_u8PinID);

void DIO_voidSetPortValue(u8 Copy_u8PortID, u8 Copy_u8Value);

void DIO_voidSetPortDirection(u8 Copy_u8PortID, u8 Copy_u8Direction);

u8 DIO_u8GetPortValue(u8 Copy_u8PortID); 


#endif
