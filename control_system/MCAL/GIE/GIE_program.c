#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "GIE_interface.h"
#include "GIE_private.h"

/* Function to enable Global Interrupts */
void GIE_voidEnable(void)
{
    SET_BIT(GIE_u8_SREG_REG, 7);
}

/* Function to disable Global Interrupts */
void GIE_voidDisable(void)
{
    CLR_BIT(GIE_u8_SREG_REG, 7);
}
