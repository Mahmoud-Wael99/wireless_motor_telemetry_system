#ifndef TIMER1_PRIVATE_H
#define TIMER1_PRIVATE_H

/* Timer1 Registers */
#define TIMER1_u8_TCCR1A_REG   *((volatile u8*)0x4F)
#define TIMER1_u8_TCCR1B_REG   *((volatile u8*)0x4E)

/* 16-bit Output Compare Register A (for PWM duty cycle) */
#define TIMER1_u16_OCR1A_REG   *((volatile u16*)0x4A)

#endif
