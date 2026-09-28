#ifndef EXTI_PRIVATE_H
#define EXTI_PRIVATE_H

/* EXTI Registers */
#define EXTI_u8_MCUCR_REG    *((volatile u8*)0x55)
#define EXTI_u8_MCUCSR_REG   *((volatile u8*)0x54)
#define EXTI_u8_GICR_REG     *((volatile u8*)0x5B)
#define EXTI_u8_GIFR_REG     *((volatile u8*)0x5A)

/* Sense Control Options */
#define EXTI_u8_LOW_LEVEL    0
#define EXTI_u8_ON_CHANGE    1
#define EXTI_u8_FALLING_EDGE 2
#define EXTI_u8_RISING_EDGE  3

#endif
