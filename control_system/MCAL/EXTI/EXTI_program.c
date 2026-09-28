#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "EXTI_interface.h"
#include "EXTI_private.h"
#include "EXTI_config.h"

/* Global pointers to functions to hold the callbacks */
static void (*EXTI_pvInt0CallBack)(void) = NULL;
static void (*EXTI_pvInt1CallBack)(void) = NULL;

void EXTI_voidInt0Init(void)
{
    /* 1. Check Sense Control for INT0 (ISC00 & ISC01 in MCUCR) */
#if EXTI0_u8_SENSE_CONTROL == EXTI_u8_LOW_LEVEL
    CLR_BIT(EXTI_u8_MCUCR_REG, 0);
    CLR_BIT(EXTI_u8_MCUCR_REG, 1);
#elif EXTI0_u8_SENSE_CONTROL == EXTI_u8_ON_CHANGE
    SET_BIT(EXTI_u8_MCUCR_REG, 0);
    CLR_BIT(EXTI_u8_MCUCR_REG, 1);
#elif EXTI0_u8_SENSE_CONTROL == EXTI_u8_FALLING_EDGE
    CLR_BIT(EXTI_u8_MCUCR_REG, 0);
    SET_BIT(EXTI_u8_MCUCR_REG, 1);
#elif EXTI0_u8_SENSE_CONTROL == EXTI_u8_RISING_EDGE
    SET_BIT(EXTI_u8_MCUCR_REG, 0);
    SET_BIT(EXTI_u8_MCUCR_REG, 1);
#endif

    /* 2. Enable Peripheral Interrupt for INT0 (PIE) */
    SET_BIT(EXTI_u8_GICR_REG, 6);
}

void EXTI_voidInt1Init(void)
{
    /* 1. Check Sense Control for INT1 (ISC10 & ISC11 in MCUCR) */
#if EXTI1_u8_SENSE_CONTROL == EXTI_u8_LOW_LEVEL
    CLR_BIT(EXTI_u8_MCUCR_REG, 2);
    CLR_BIT(EXTI_u8_MCUCR_REG, 3);
#elif EXTI1_u8_SENSE_CONTROL == EXTI_u8_ON_CHANGE
    SET_BIT(EXTI_u8_MCUCR_REG, 2);
    CLR_BIT(EXTI_u8_MCUCR_REG, 3);
#elif EXTI1_u8_SENSE_CONTROL == EXTI_u8_FALLING_EDGE
    CLR_BIT(EXTI_u8_MCUCR_REG, 2);
    SET_BIT(EXTI_u8_MCUCR_REG, 3);
#elif EXTI1_u8_SENSE_CONTROL == EXTI_u8_RISING_EDGE
    SET_BIT(EXTI_u8_MCUCR_REG, 2);
    SET_BIT(EXTI_u8_MCUCR_REG, 3);
#endif

    /* 2. Enable Peripheral Interrupt for INT1 (PIE) */
    SET_BIT(EXTI_u8_GICR_REG, 7);
}

u8 EXTI_u8Int0SetCallBack(void (*Copy_pvCallBackFunc)(void))
{
    u8 Local_u8ErrorState = 0; // 0 means OK
    if(Copy_pvCallBackFunc != NULL)
    {
        EXTI_pvInt0CallBack = Copy_pvCallBackFunc;
    }
    else
    {
        Local_u8ErrorState = 1; // Null pointer passed
    }
    return Local_u8ErrorState;
}

u8 EXTI_u8Int1SetCallBack(void (*Copy_pvCallBackFunc)(void))
{
    u8 Local_u8ErrorState = 0;
    if(Copy_pvCallBackFunc != NULL)
    {
        EXTI_pvInt1CallBack = Copy_pvCallBackFunc;
    }
    else
    {
        Local_u8ErrorState = 1;
    }
    return Local_u8ErrorState;
}

/* ISR for INT0 (Encoder Phase A) */
void __vector_1(void) __attribute__((signal));
void __vector_1(void)
{
    if(EXTI_pvInt0CallBack != NULL)
    {
        EXTI_pvInt0CallBack();
    }
}

/* ISR for INT1 (MPU6050 Data Ready) */
void __vector_2(void) __attribute__((signal));
void __vector_2(void)
{
    if(EXTI_pvInt1CallBack != NULL)
    {
        EXTI_pvInt1CallBack();
    }
}
