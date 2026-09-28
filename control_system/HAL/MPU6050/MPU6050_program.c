#include "../../LIB/CLOCK.h"
#include <util/delay.h>
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/I2C_SW/I2C_SW_interface.h"

#include "MPU6050_interface.h"
#include "MPU6050_config.h"
#include "MPU6050_private.h"

void MPU6050_voidInit(void)
{
    u8 Local_u8Try;
    u8 Local_u8Nack;

    /* 1. Initialize the Software I2C bus */
    I2C_SW_voidInit();

    /* 2. Let the sensor finish its own power-up */
    _delay_ms(MPU6050_u8_POWERUP_DELAY_MS);

    /* 3. Wake up MPU6050 (write 0x00 to PWR_MGMT_1 clears the sleep bit).
          Retry a few times if any byte is not acknowledged. */
    for (Local_u8Try = 0; Local_u8Try < MPU6050_u8_INIT_RETRIES; Local_u8Try++)
    {
        I2C_SW_voidStart();
        Local_u8Nack  = I2C_SW_u8WriteByte(MPU6050_u8_ADDRESS_WRITE);
        Local_u8Nack |= I2C_SW_u8WriteByte(MPU6050_REG_PWR_MGMT_1);
        Local_u8Nack |= I2C_SW_u8WriteByte(0x00);
        I2C_SW_voidStop();

        if (Local_u8Nack == I2C_SW_ACK)
        {
            break;
        }

        _delay_ms(10);
    }
}

void MPU6050_voidGetAccelRaw(s16* Copy_ps16AccelX, s16* Copy_ps16AccelY, s16* Copy_ps16AccelZ)
{
    u8 Local_u8Data[6];
    u8 Local_u8Iterator;

    /* 1. Point the MPU6050 to the ACCEL_XOUT_H register */
    I2C_SW_voidStart();
    if (I2C_SW_u8WriteByte(MPU6050_u8_ADDRESS_WRITE) != I2C_SW_ACK)
    {
        /* Sensor not answering: release the bus and report zeros */
        I2C_SW_voidStop();
        *Copy_ps16AccelX = 0;
        *Copy_ps16AccelY = 0;
        *Copy_ps16AccelZ = 0;
        return;
    }
    I2C_SW_u8WriteByte(MPU6050_REG_ACCEL_XOUT_H);

    /* 2. Repeated START to switch to read mode */
    I2C_SW_voidStart();
    if (I2C_SW_u8WriteByte(MPU6050_u8_ADDRESS_READ) != I2C_SW_ACK)
    {
        I2C_SW_voidStop();
        *Copy_ps16AccelX = 0;
        *Copy_ps16AccelY = 0;
        *Copy_ps16AccelZ = 0;
        return;
    }

    /* 3. Read the first 5 bytes and acknowledge each (ACK) */
    for (Local_u8Iterator = 0; Local_u8Iterator < 5; Local_u8Iterator++)
    {
        Local_u8Data[Local_u8Iterator] = I2C_SW_u8ReadByte(I2C_SW_ACK);
    }

    /* 4. Read the 6th byte and send NACK to end communication */
    Local_u8Data[5] = I2C_SW_u8ReadByte(I2C_SW_NACK);

    I2C_SW_voidStop();

    /* 5. Combine High and Low bytes (cast to u16 BEFORE shifting: a u8 promotes
          to a signed 16-bit int on AVR and shifting into the sign bit is UB) */
    *Copy_ps16AccelX = (s16)(((u16)Local_u8Data[0] << 8) | Local_u8Data[1]);
    *Copy_ps16AccelY = (s16)(((u16)Local_u8Data[2] << 8) | Local_u8Data[3]);
    *Copy_ps16AccelZ = (s16)(((u16)Local_u8Data[4] << 8) | Local_u8Data[5]);
}
