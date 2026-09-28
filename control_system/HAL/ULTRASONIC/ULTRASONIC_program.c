#include "../../LIB/CLOCK.h"
#include <avr/io.h>
#include <util/delay.h>
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/DIO/DIO_interface.h"

#include "ULTRASONIC_interface.h"
#include "ULTRASONIC_config.h"

/*
   Timer0 is used as a free-running stopwatch (Normal mode, prescaler 64).
   One tick = 64 / F_CPU seconds  (8 us at 8 MHz). TCNT0 is only 8 bits, so
   the overflow flag (TOV0) is polled and counted in software.
   Timer1 is NOT touched: it generates the motor PWM.
*/
#define ULTRASONIC_u32_TIMER0_PRESCALER   64UL
#define ULTRASONIC_TICKS_TO_US(TICKS)     (((u32)(TICKS) * ULTRASONIC_u32_TIMER0_PRESCALER) / (F_CPU / 1000000UL))

static u16 Global_u16Overflows = 0;

static void ULTRASONIC_voidTimerStart(void)
{
    TCCR0 = 0;                          /* stop, Normal mode            */
    TCNT0 = 0;
    TIFR  = (1 << TOV0);                /* clear overflow flag (write 1) */
    Global_u16Overflows = 0;
    TCCR0 = (1 << CS01) | (1 << CS00);  /* clk/64 -> start              */
}

static void ULTRASONIC_voidTimerStop(void)
{
    TCCR0 = 0;
}

/* Elapsed timer ticks since ULTRASONIC_voidTimerStart() */
static u32 ULTRASONIC_u32ReadTicks(void)
{
    u8 Local_u8Count = TCNT0;

    if (TIFR & (1 << TOV0))
    {
        TIFR = (1 << TOV0);             /* clear flag */
        Global_u16Overflows++;
        Local_u8Count = TCNT0;          /* re-read so count and overflows agree */
    }

    return (((u32)Global_u16Overflows) << 8) | Local_u8Count;
}

u16 ULTRASONIC_u16GetDistance(void)
{
    u32 Local_u32Ticks;
    u32 Local_u32EchoUs;

    /* 1. Send a clean 10 us trigger pulse */
    DIO_voidSetPinValue(ULTRASONIC_u8_TRIG_PORT, ULTRASONIC_u8_TRIG_PIN, DIO_u8_LOW);
    _delay_us(2);
    DIO_voidSetPinValue(ULTRASONIC_u8_TRIG_PORT, ULTRASONIC_u8_TRIG_PIN, DIO_u8_HIGH);
    _delay_us(10);
    DIO_voidSetPinValue(ULTRASONIC_u8_TRIG_PORT, ULTRASONIC_u8_TRIG_PIN, DIO_u8_LOW);

    /* 2. Wait for the Echo pin to go HIGH (start of pulse) */
    ULTRASONIC_voidTimerStart();
    while (DIO_u8GetPinValue(ULTRASONIC_u8_ECHO_PORT, ULTRASONIC_u8_ECHO_PIN) == DIO_u8_LOW)
    {
        if (ULTRASONIC_TICKS_TO_US(ULTRASONIC_u32ReadTicks()) > ULTRASONIC_u32_ECHO_START_TIMEOUT_US)
        {
            ULTRASONIC_voidTimerStop();
            return 0;                   /* no response (sensor missing / wiring) */
        }
    }

    /* 3. Measure how long Echo stays HIGH */
    ULTRASONIC_voidTimerStart();
    while (DIO_u8GetPinValue(ULTRASONIC_u8_ECHO_PORT, ULTRASONIC_u8_ECHO_PIN) == DIO_u8_HIGH)
    {
        if (ULTRASONIC_TICKS_TO_US(ULTRASONIC_u32ReadTicks()) > ULTRASONIC_u32_ECHO_MAX_US)
        {
            ULTRASONIC_voidTimerStop();
            return 0;                   /* nothing in range */
        }
    }
    Local_u32Ticks = ULTRASONIC_u32ReadTicks();
    ULTRASONIC_voidTimerStop();

    /* 4. Echo width (us) -> distance (cm) */
    Local_u32EchoUs = ULTRASONIC_TICKS_TO_US(Local_u32Ticks);

    return (u16)(Local_u32EchoUs / ULTRASONIC_u8_US_PER_CM);
}
