#ifndef I2C_SW_INTERFACE_H
#define I2C_SW_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

#define I2C_SW_ACK      0
#define I2C_SW_NACK     1

void I2C_SW_voidInit(void);
void I2C_SW_voidStart(void);
void I2C_SW_voidStop(void);

/* Returns the ACK/NACK bit from the slave (0 = ACK, 1 = NACK) */
u8 I2C_SW_u8WriteByte(u8 Copy_u8Data);

/* Reads a byte and sends an ACK (0) or NACK (1) back to the slave */
u8 I2C_SW_u8ReadByte(u8 Copy_u8Ack);

#endif
