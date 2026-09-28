#include "../../LIB/CLOCK.h"
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "UART_interface.h"
#include "UART_private.h"
#include "UART_config.h"

/*
   UBRR = F_CPU / (16 * BAUD) - 1   (normal speed, U2X = 0), rounded to nearest.
   8 MHz  @ 9600 -> 51      16 MHz @ 9600 -> 103
   This follows F_CPU automatically, so the baud rate can no longer silently
   drift when the clock setting changes.
*/
#define UART_u16_UBRR_VALUE  ((u16)((((F_CPU) + (8UL * UART_u32_BAUD_RATE)) / (16UL * UART_u32_BAUD_RATE)) - 1UL))

void UART_voidInit(void)
{
    /* 1. Set the baud rate (computed from F_CPU and UART_u32_BAUD_RATE) */
    /* UBRRH shares its address with UCSRC: bit 7 (URSEL) = 0 selects UBRRH */
    UART_u8_UBRRH_REG = (u8)(UART_u16_UBRR_VALUE >> 8);
    UART_u8_UBRRL_REG = (u8)(UART_u16_UBRR_VALUE & 0xFF);

    /* 2. Enable Receiver (RXEN) and Transmitter (TXEN) */
    SET_BIT(UART_u8_UCSRB_REG, 3); // TXEN
    SET_BIT(UART_u8_UCSRB_REG, 4); // RXEN

    /* 3. Set Frame Format: 8 data bits, 1 stop bit, no parity */
    /* URSEL=1 (To access UCSRC), UCSZ1=1, UCSZ0=1 */
    UART_u8_UCSRC_REG = (1 << 7) | (1 << 2) | (1 << 1);
}

void UART_voidSendByte(u8 Copy_u8Data)
{
    /* Wait for empty transmit buffer by checking UDRE flag (Bit 5 in UCSRA) */
    while(GET_BIT(UART_u8_UCSRA_REG, 5) == 0)
    {
        // Wait
    }

    /* Put data into buffer, which sends the data */
    UART_u8_UDR_REG = Copy_u8Data;
}

u8 UART_u8ReceiveByte(void)
{
    /* Wait for data to be received by checking RXC flag (Bit 7 in UCSRA) */
    while(GET_BIT(UART_u8_UCSRA_REG, 7) == 0)
    {
        // Wait
    }

    /* Get and return received data from buffer */
    return UART_u8_UDR_REG;
}

void UART_voidSendString(const char* Copy_pcString)
{
    u8 Local_u8Counter = 0;

    /* Loop until the null terminator is reached */
    while(Copy_pcString[Local_u8Counter] != '\0')
    {
        UART_voidSendByte(Copy_pcString[Local_u8Counter]);
        Local_u8Counter++;
    }
}
