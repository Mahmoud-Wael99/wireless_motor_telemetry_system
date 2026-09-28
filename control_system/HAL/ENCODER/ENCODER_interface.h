#ifndef ENCODER_INTERFACE_H
#define ENCODER_INTERFACE_H

/* Direction values returned by ENCODER_u8GetDirection() */
#define ENCODER_u8_DIR_FORWARD    0
#define ENCODER_u8_DIR_REVERSE    1

/*
 * Initializes Phase A (INT0) and Phase B pins as inputs, registers the ISR
 * callback, and initializes the INT0 hardware.
 * NOTE: Global interrupts must be enabled by the application afterwards.
 */
void ENCODER_voidInit(void);

/* Returns pulses counted since the last reset (atomic read) */
u32 ENCODER_u32GetPulseCount(void);

/* Clears the pulse counter (atomic write) */
void ENCODER_voidResetPulseCount(void);

/* Returns ENCODER_u8_DIR_FORWARD or ENCODER_u8_DIR_REVERSE (last sampled) */
u8 ENCODER_u8GetDirection(void);

/*
 * Returns the speed in RPM measured over the given window, then clears the
 * pulse counter so the next window starts fresh.
 * Call this periodically, passing the actual period in milliseconds.
 * Returns 0 if Copy_u16TimePeriodMs is 0.
 */
u16 ENCODER_u16CalculateRPM(u16 Copy_u16TimePeriodMs);

#endif
