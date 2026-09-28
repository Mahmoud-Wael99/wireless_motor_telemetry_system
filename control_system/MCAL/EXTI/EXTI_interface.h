#ifndef EXTI_INTERFACE_H
#define EXTI_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

void EXTI_voidInt0Init(void);
void EXTI_voidInt1Init(void);

/* Pointer to function to hold the application callback for INT0 (Encoder) */
u8 EXTI_u8Int0SetCallBack(void (*Copy_pvCallBackFunc)(void));

/* Pointer to function to hold the application callback for INT1 (MPU6050) */
u8 EXTI_u8Int1SetCallBack(void (*Copy_pvCallBackFunc)(void));

#endif
