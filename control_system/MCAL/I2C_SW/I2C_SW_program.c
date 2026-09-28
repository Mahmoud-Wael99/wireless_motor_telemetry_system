#include "../../LIB/CLOCK.h"
#include <util/delay.h>
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../DIO/DIO_interface.h"

#include "I2C_SW_interface.h"
#include "I2C_SW_config.h"

static void I2C_SW_voidHalfClockDelay(void)
{
    _delay_us(I2C_SW_u8_DELAY_US);
}

/* ------------------------------------------------------------------------ */
/* Low-level line control                                                    */
/* ------------------------------------------------------------------------ */
#if I2C_SW_u8_OPEN_DRAIN == 1

/* Release = input without pull-up -> external pull-up makes the line HIGH */
static void I2C_SW_voidLineHigh(u8 Copy_u8Port, u8 Copy_u8Pin)
{
    DIO_voidSetPinDirection(Copy_u8Port, Copy_u8Pin, DIO_u8_INITIAL_INPUT);
}

/* Pull low = output with the PORT bit at 0 (it is kept at 0 the whole time) */
static void I2C_SW_voidLineLow(u8 Copy_u8Port, u8 Copy_u8Pin)
{
    DIO_voidSetPinDirection(Copy_u8Port, Copy_u8Pin, DIO_u8_INITIAL_OUTPUT);
}

#else

static void I2C_SW_voidLineHigh(u8 Copy_u8Port, u8 Copy_u8Pin)
{
    DIO_voidSetPinValue(Copy_u8Port, Copy_u8Pin, DIO_u8_HIGH);
    DIO_voidSetPinDirection(Copy_u8Port, Copy_u8Pin, DIO_u8_INITIAL_OUTPUT);
}

static void I2C_SW_voidLineLow(u8 Copy_u8Port, u8 Copy_u8Pin)
{
    DIO_voidSetPinValue(Copy_u8Port, Copy_u8Pin, DIO_u8_LOW);
    DIO_voidSetPinDirection(Copy_u8Port, Copy_u8Pin, DIO_u8_INITIAL_OUTPUT);
}

#endif

#define I2C_SW_SCL_HIGH()   I2C_SW_voidLineHigh(I2C_SW_u8_SCL_PORT, I2C_SW_u8_SCL_PIN)
#define I2C_SW_SCL_LOW()    I2C_SW_voidLineLow (I2C_SW_u8_SCL_PORT, I2C_SW_u8_SCL_PIN)
#define I2C_SW_SDA_HIGH()   I2C_SW_voidLineHigh(I2C_SW_u8_SDA_PORT, I2C_SW_u8_SDA_PIN)
#define I2C_SW_SDA_LOW()    I2C_SW_voidLineLow (I2C_SW_u8_SDA_PORT, I2C_SW_u8_SDA_PIN)

/* Give the SDA line to the slave (so it can send data / ACK) */
static void I2C_SW_voidSdaRelease(void)
{
#if I2C_SW_u8_OPEN_DRAIN == 1
    I2C_SW_voidLineHigh(I2C_SW_u8_SDA_PORT, I2C_SW_u8_SDA_PIN);
#else
    DIO_voidSetPinDirection(I2C_SW_u8_SDA_PORT, I2C_SW_u8_SDA_PIN, DIO_u8_INITIAL_INPUT);
#endif
}

/* ------------------------------------------------------------------------ */
/* Public API                                                                */
/* ------------------------------------------------------------------------ */
void I2C_SW_voidInit(void)
{
#if I2C_SW_u8_OPEN_DRAIN == 1
    /* PORT bits must be 0 so that "output" always means "pull low" */
    DIO_voidSetPinValue(I2C_SW_u8_SCL_PORT, I2C_SW_u8_SCL_PIN, DIO_u8_LOW);
    DIO_voidSetPinValue(I2C_SW_u8_SDA_PORT, I2C_SW_u8_SDA_PIN, DIO_u8_LOW);
#endif
    /* Idle bus: both lines HIGH */
    I2C_SW_SCL_HIGH();
    I2C_SW_SDA_HIGH();
    I2C_SW_voidHalfClockDelay();
}

/*
   START and REPEATED START.
   Works from ANY bus state (idle, or right after a byte when SCL is LOW):
   first make SDA HIGH, then SCL HIGH, and only then pull SDA LOW while SCL is
   HIGH. (The old version pulled SDA low while SCL was already low, so no START
   condition existed and the MPU6050 never switched to read mode.)
*/
void I2C_SW_voidStart(void)
{
    I2C_SW_SDA_HIGH();
    I2C_SW_voidHalfClockDelay();
    I2C_SW_SCL_HIGH();
    I2C_SW_voidHalfClockDelay();

    I2C_SW_SDA_LOW();                 /* falling edge on SDA while SCL is HIGH */
    I2C_SW_voidHalfClockDelay();
    I2C_SW_SCL_LOW();
    I2C_SW_voidHalfClockDelay();
}

void I2C_SW_voidStop(void)
{
    /* Called with SCL LOW */
    I2C_SW_SDA_LOW();
    I2C_SW_voidHalfClockDelay();

    I2C_SW_SCL_HIGH();
    I2C_SW_voidHalfClockDelay();

    I2C_SW_SDA_HIGH();                /* rising edge on SDA while SCL is HIGH */
    I2C_SW_voidHalfClockDelay();
}

u8 I2C_SW_u8WriteByte(u8 Copy_u8Data)
{
    u8 Local_u8Iterator;
    u8 Local_u8Ack;

    /* Write 8 bits, MSB first (SCL is LOW on entry) */
    for (Local_u8Iterator = 0; Local_u8Iterator < 8; Local_u8Iterator++)
    {
        if (GET_BIT(Copy_u8Data, (7 - Local_u8Iterator)))
        {
            I2C_SW_SDA_HIGH();
        }
        else
        {
            I2C_SW_SDA_LOW();
        }

        /* Clock pulse */
        I2C_SW_voidHalfClockDelay();
        I2C_SW_SCL_HIGH();
        I2C_SW_voidHalfClockDelay();
        I2C_SW_SCL_LOW();
    }

    /* 9th clock: release SDA so the slave can pull it low (ACK) */
    I2C_SW_voidSdaRelease();
    I2C_SW_voidHalfClockDelay();
    I2C_SW_SCL_HIGH();
    I2C_SW_voidHalfClockDelay();

    Local_u8Ack = DIO_u8GetPinValue(I2C_SW_u8_SDA_PORT, I2C_SW_u8_SDA_PIN);   /* 0 = ACK, 1 = NACK */

    I2C_SW_SCL_LOW();

    return Local_u8Ack;
}

u8 I2C_SW_u8ReadByte(u8 Copy_u8Ack)
{
    u8 Local_u8Iterator;
    u8 Local_u8Data = 0;

    /* Slave drives SDA while we clock */
    I2C_SW_voidSdaRelease();

    /* Read 8 bits, MSB first */
    for (Local_u8Iterator = 0; Local_u8Iterator < 8; Local_u8Iterator++)
    {
        I2C_SW_voidHalfClockDelay();
        I2C_SW_SCL_HIGH();
        I2C_SW_voidHalfClockDelay();

        if (DIO_u8GetPinValue(I2C_SW_u8_SDA_PORT, I2C_SW_u8_SDA_PIN) == DIO_u8_HIGH)
        {
            SET_BIT(Local_u8Data, (7 - Local_u8Iterator));
        }

        I2C_SW_SCL_LOW();
    }

    /* Send ACK (SDA low) or NACK (SDA high) to the slave */
    if (Copy_u8Ack == I2C_SW_ACK)
    {
        I2C_SW_SDA_LOW();
    }
    else
    {
        I2C_SW_SDA_HIGH();
    }

    I2C_SW_voidHalfClockDelay();
    I2C_SW_SCL_HIGH();
    I2C_SW_voidHalfClockDelay();
    I2C_SW_SCL_LOW();

    /* Hand SDA back to the slave for the next byte */
    I2C_SW_voidSdaRelease();

    return Local_u8Data;
}
