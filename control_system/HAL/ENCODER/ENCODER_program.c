#include <avr/io.h>              /* SREG, for atomic (critical section) access */

#include "../../LIB/STD_TYPES.h"

#include "../../MCAL/DIO/DIO_interface.h"
#include "../../MCAL/EXTI/EXTI_interface.h"

#include "ENCODER_interface.h"
#include "ENCODER_config.h"

/* Shared with the ISR, so they must be volatile */
static volatile u32 Global_u32PulseCount = 0;
static volatile u8  Global_u8Direction   = ENCODER_u8_DIR_FORWARD;

/*
   Critical section helpers.
   A u32 is 4 bytes on the 8-bit AVR, so reading/writing it takes several
   instructions. INT0 could fire in the middle and corrupt the value or lose
   pulses, so interrupts are disabled around every access made outside the ISR.
   The previous SREG value is saved and restored so the helpers are safe even
   if they are called with interrupts already disabled.
*/
#define ENCODER_ENTER_CRITICAL(SAVED)   do { (SAVED) = SREG; __asm__ __volatile__("cli" ::: "memory"); } while (0)
#define ENCODER_EXIT_CRITICAL(SAVED)    do { SREG = (SAVED); __asm__ __volatile__("" ::: "memory"); } while (0)

/* Private ISR function passed to the EXTI driver (runs on Phase A rising edge) */
static void ENCODER_voidPulseISR(void)
{
    Global_u32PulseCount++;

    /*
       Phase A just went HIGH (rising edge):
       Phase B LOW  -> Forward
       Phase B HIGH -> Reverse
       (Swap the two branches if "forward" is reversed on your motor.)
    */
    if (DIO_u8GetPinValue(ENCODER_u8_PHASE_B_PORT, ENCODER_u8_PHASE_B_PIN) == DIO_u8_LOW)
    {
        Global_u8Direction = ENCODER_u8_DIR_FORWARD;
    }
    else
    {
        Global_u8Direction = ENCODER_u8_DIR_REVERSE;
    }
}

void ENCODER_voidInit(void)
{
    /* Phase A (INT0 / PD2) and Phase B as inputs */
    DIO_voidSetPinDirection(ENCODER_u8_PHASE_A_PORT, ENCODER_u8_PHASE_A_PIN, DIO_u8_INITIAL_INPUT);
    DIO_voidSetPinDirection(ENCODER_u8_PHASE_B_PORT, ENCODER_u8_PHASE_B_PIN, DIO_u8_INITIAL_INPUT);

#if ENCODER_u8_PULLUP_ENABLE == 1
    /* Writing HIGH to an input pin enables its internal pull-up */
    DIO_voidSetPinValue(ENCODER_u8_PHASE_A_PORT, ENCODER_u8_PHASE_A_PIN, DIO_u8_HIGH);
    DIO_voidSetPinValue(ENCODER_u8_PHASE_B_PORT, ENCODER_u8_PHASE_B_PIN, DIO_u8_HIGH);
#endif

    /* Link the private ISR to the MCAL EXTI driver.
       Assumes a non-zero return value means an error; only enable the
       interrupt if the callback was registered successfully. */
    if (EXTI_u8Int0SetCallBack(&ENCODER_voidPulseISR) == 0)
    {
        EXTI_voidInt0Init();
    }
}

u32 ENCODER_u32GetPulseCount(void)
{
    u32 Local_u32Count;
    u8  Local_u8SavedSREG;

    ENCODER_ENTER_CRITICAL(Local_u8SavedSREG);
    Local_u32Count = Global_u32PulseCount;
    ENCODER_EXIT_CRITICAL(Local_u8SavedSREG);

    return Local_u32Count;
}

void ENCODER_voidResetPulseCount(void)
{
    u8 Local_u8SavedSREG;

    ENCODER_ENTER_CRITICAL(Local_u8SavedSREG);
    Global_u32PulseCount = 0;
    ENCODER_EXIT_CRITICAL(Local_u8SavedSREG);
}

u8 ENCODER_u8GetDirection(void)
{
    /* Single-byte read is atomic on AVR */
    return Global_u8Direction;
}

u16 ENCODER_u16CalculateRPM(u16 Copy_u16TimePeriodMs)
{
    u32 Local_u32Pulses;
    u32 Local_u32Divisor;
    u32 Local_u32MaxPulses;
    u32 Local_u32RPM;
    u8  Local_u8SavedSREG;

    /* Guard against divide-by-zero */
    if (Copy_u16TimePeriodMs == 0)
    {
        return 0;
    }

    /* Atomic read-and-clear: no pulse can be lost between the two steps */
    ENCODER_ENTER_CRITICAL(Local_u8SavedSREG);
    Local_u32Pulses = Global_u32PulseCount;
    Global_u32PulseCount = 0;
    ENCODER_EXIT_CRITICAL(Local_u8SavedSREG);

    /*
       RPM = (Pulses / Time_Seconds) * 60 / PPR
           = (Pulses * 60000) / (Time_Ms * PPR)
       Divisor max = 65535 * 374 (fits in u32).
    */
    Local_u32Divisor = (u32)Copy_u16TimePeriodMs * ENCODER_u16_PPR;

    /* Clamp pulses so (Pulses * 60000 + Divisor/2) cannot overflow a u32 */
    Local_u32MaxPulses = (0xFFFFFFFFUL - (Local_u32Divisor / 2UL)) / 60000UL;
    if (Local_u32Pulses > Local_u32MaxPulses)
    {
        Local_u32Pulses = Local_u32MaxPulses;
    }

    /* Rounded (not truncated) integer division */
    Local_u32RPM = ((Local_u32Pulses * 60000UL) + (Local_u32Divisor / 2UL)) / Local_u32Divisor;

    /* Saturate to the u16 return type */
    if (Local_u32RPM > 0xFFFFUL)
    {
        Local_u32RPM = 0xFFFFUL;
    }

    return (u16)Local_u32RPM;
}
