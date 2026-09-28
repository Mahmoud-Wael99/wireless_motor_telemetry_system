#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

void UART_voidInit(void);

void UART_voidSendByte(u8 Copy_u8Data);
u8 UART_u8ReceiveByte(void);

void UART_voidSendString(const char* Copy_pcString);

#endif
