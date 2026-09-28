/* Header Files Inclusions */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "DIO_interface.h"
#include "DIO_private.h"
#include "DIO_config.h"


/* Function: DIO_voidSetPinValue */
void DIO_voidSetPinValue(u8 Copy_u8PortID, u8 Copy_u8PinID, u8 Copy_u8Value)
{
	/* Input Validation */
	if((Copy_u8PortID > DIO_u8_PORTD) || (Copy_u8PinID > DIO_u8_PIN7))
	{
		// Do nothing
	}
	else
	{
		switch(Copy_u8PortID)
		{
			case DIO_u8_PORTA: switch(Copy_u8Value)
								{
									/* Sets Pin's value as LOW */
									case DIO_u8_LOW: CLR_BIT(DIO_u8_PORTA_REG,Copy_u8PinID);										
										break;
									/* Sets Pin's value as High */
									case DIO_u8_HIGH: SET_BIT(DIO_u8_PORTA_REG,Copy_u8PinID);
										break;
								}
			break;
			
			case DIO_u8_PORTB: switch(Copy_u8Value)
								{
									/* Sets Pin's value as LOW */
									case DIO_u8_LOW: CLR_BIT(DIO_u8_PORTB_REG,Copy_u8PinID);										
										break;
									/* Sets Pin's value as High */
									case DIO_u8_HIGH: SET_BIT(DIO_u8_PORTB_REG,Copy_u8PinID);
										break;
								}
			break;
			
			case DIO_u8_PORTC: switch(Copy_u8Value)
								{
									/* Sets Pin's value as LOW */
									case DIO_u8_LOW: CLR_BIT(DIO_u8_PORTC_REG,Copy_u8PinID);										
										break;
									/* Sets Pin's value as High */
									case DIO_u8_HIGH: SET_BIT(DIO_u8_PORTC_REG,Copy_u8PinID);
										break;
								}
			break;
			
			case DIO_u8_PORTD: switch(Copy_u8Value)
								{
									/* Sets Pin's value as LOW */
									case DIO_u8_LOW: CLR_BIT(DIO_u8_PORTD_REG,Copy_u8PinID);										
										break;
									/* Sets Pin's value as High */
									case DIO_u8_HIGH: SET_BIT(DIO_u8_PORTD_REG,Copy_u8PinID);
										break;
								}
			break;
		}
	}
}

/* Function: DIO_voidSetPinDirection */
void DIO_voidSetPinDirection(u8 Copy_u8PortID, u8 Copy_u8PinID, u8 Copy_u8Direction)
{
	/* Input Validation */
	if((Copy_u8PortID > DIO_u8_PORTD) || (Copy_u8PinID > DIO_u8_PIN7))
	{
		// Do nothing
	}
	else
	{
		switch(Copy_u8PortID)
		{
			case DIO_u8_PORTA: switch(Copy_u8Direction)
								{
									/* Sets Pin's value as LOW */
									case DIO_u8_INITIAL_INPUT: CLR_BIT(DIO_u8_DDRA_REG,Copy_u8PinID);										
										break;
									/* Sets Pin's value as High */
									case DIO_u8_INITIAL_OUTPUT: SET_BIT(DIO_u8_DDRA_REG,Copy_u8PinID);
										break;
								}
			break;
			
			case DIO_u8_PORTB: switch(Copy_u8Direction)
								{
									/* Sets Pin's value as LOW */
									case DIO_u8_INITIAL_INPUT: CLR_BIT(DIO_u8_DDRB_REG,Copy_u8PinID);										
										break;
									/* Sets Pin's value as High */
									case DIO_u8_INITIAL_OUTPUT: SET_BIT(DIO_u8_DDRB_REG,Copy_u8PinID);
										break;
								}
			break;
			
			case DIO_u8_PORTC: switch(Copy_u8Direction)
								{
									/* Sets Pin's value as LOW */
									case DIO_u8_INITIAL_INPUT: CLR_BIT(DIO_u8_DDRC_REG,Copy_u8PinID);										
										break;
									/* Sets Pin's value as High */
									case DIO_u8_INITIAL_OUTPUT: SET_BIT(DIO_u8_DDRC_REG,Copy_u8PinID);
										break;
								}
			break;
			
			case DIO_u8_PORTD: switch(Copy_u8Direction)
								{
									/* Sets Pin's value as LOW */
									case DIO_u8_INITIAL_INPUT: CLR_BIT(DIO_u8_DDRD_REG,Copy_u8PinID);										
										break;
									/* Sets Pin's value as High */
									case DIO_u8_INITIAL_OUTPUT: SET_BIT(DIO_u8_DDRD_REG,Copy_u8PinID);
										break;
								}
			break;
		}
	}
}


/* Function: DIO_u8GetPinValue */
u8 DIO_u8GetPinValue(u8 Copy_u8PortID, u8 Copy_u8PinID)
{
	u8 LOCAL_u8PinState = DIO_u8_LOW;
	
	/* Input Validation */
	if((Copy_u8PortID > DIO_u8_PORTD) || (Copy_u8PinID > DIO_u8_PIN7))
	{
		// Do nothing
	}
	else
	{
		switch(Copy_u8PortID)
		{
			case DIO_u8_PORTA:
				LOCAL_u8PinState = GET_BIT(DIO_u8_PINA_REG,Copy_u8PinID);
			break;
			
			case DIO_u8_PORTB:
				LOCAL_u8PinState = GET_BIT(DIO_u8_PINB_REG,Copy_u8PinID);

			break;
			
			case DIO_u8_PORTC:
				LOCAL_u8PinState = GET_BIT(DIO_u8_PINC_REG,Copy_u8PinID);

			break;
			
			case DIO_u8_PORTD:
				LOCAL_u8PinState = GET_BIT(DIO_u8_PIND_REG,Copy_u8PinID);
			break;
			
			default:
			/* Invalid Port */
				break;
		}
	}
	return LOCAL_u8PinState;
}

/* Function: DIO_voidSetPortValue */
void DIO_voidSetPortValue(u8 Copy_u8PortID, u8 Copy_u8Value)
{
	if(Copy_u8PortID > DIO_u8_PORTD)
	{
		
	}
	else
	{
		switch(Copy_u8PortID)
		{
			case DIO_u8_PORTA:
				DIO_u8_PORTA_REG = Copy_u8Value;
			break;
			
			case DIO_u8_PORTB:
				DIO_u8_PORTB_REG = Copy_u8Value;
			break;
				
			case DIO_u8_PORTC:
				DIO_u8_PORTC_REG = Copy_u8Value;
			break;
			
			case DIO_u8_PORTD:
				DIO_u8_PORTD_REG = Copy_u8Value;
			break;
			
			default:
			/* Invalid Port Number */
			break;
		}
	}
}

/* Function: DIO_voidSetPortDirection */
void DIO_voidSetPortDirection(u8 Copy_u8PortID, u8 Copy_u8Direction)
{
		/* Input Validation */
	if((Copy_u8PortID > DIO_u8_PORTD))
	{
		// Do nothing
	}
	else
	{
		switch(Copy_u8PortID)
		{
			case DIO_u8_PORTA:
				DIO_u8_DDRA_REG = Copy_u8Direction;
			break;
			
			case DIO_u8_PORTB: 				
				DIO_u8_DDRB_REG = Copy_u8Direction;
			break;
			
			case DIO_u8_PORTC:
				DIO_u8_DDRC_REG = Copy_u8Direction;
			break;
			
			case DIO_u8_PORTD:
				DIO_u8_DDRD_REG = Copy_u8Direction;
			break;
			
			default:
				/* Invalid Port Number */
				break;
		}
	}
}

/* Function: DIO_voidGetPortValue */
u8 DIO_u8GetPortValue(u8 Copy_u8PortID)
{
	u8 Local_u8PortValue = DIO_u8_LOW;
	
	switch(Copy_u8PortID)
	{
		case DIO_u8_PORTA:
			Local_u8PortValue = DIO_u8_PINA_REG;
		break;
		
		case DIO_u8_PORTB:
			Local_u8PortValue = DIO_u8_PINB_REG;		
		break;
			
		case DIO_u8_PORTC:
			Local_u8PortValue = DIO_u8_PINC_REG;
		break;
		
		case DIO_u8_PORTD:
			Local_u8PortValue = DIO_u8_PIND_REG;
		break;
		
		default:
		 /* Invalid Port Number */
		 break;
	}
	return Local_u8PortValue;
}
