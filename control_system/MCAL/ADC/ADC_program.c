#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "ADC_interface.h"
#include "ADC_private.h"
#include "ADC_config.h"

void ADC_voidInit(void)
{
    /* 1. Select VREF (AVCC) -> REFS0=1, REFS1=0 in ADMUX */
    SET_BIT(ADC_u8_ADMUX_REG, 6);
    CLR_BIT(ADC_u8_ADMUX_REG, 7);

    /* 2. Right Adjust Result -> ADLAR=0 (so we read the full 10-bit value easily) */
    CLR_BIT(ADC_u8_ADMUX_REG, 5);

    /* 3. Set Prescaler to 128: ADC clock = F_CPU/128 = 62.5 kHz @ 8 MHz (125 kHz @ 16 MHz).
          Both are inside the 50..200 kHz range required for full 10-bit accuracy. */
    SET_BIT(ADC_u8_ADCSRA_REG, 0);
    SET_BIT(ADC_u8_ADCSRA_REG, 1);
    SET_BIT(ADC_u8_ADCSRA_REG, 2);

    /* 4. Enable ADC -> ADEN=1 */
    SET_BIT(ADC_u8_ADCSRA_REG, 7);
}

u16 ADC_u16GetChannelReading(u8 Copy_u8Channel)
{
    /* 1. Clear the MUX bits in ADMUX (Bits 0-4) to prepare for the new channel */
    ADC_u8_ADMUX_REG &= 0xE0;

    /* 2. Set the requested channel (0 to 7) */
    ADC_u8_ADMUX_REG |= Copy_u8Channel;

    /* 3. Start Conversion -> ADSC=1 */
    SET_BIT(ADC_u8_ADCSRA_REG, 6);

    /* 4. Wait for conversion to complete by polling the ADC Interrupt Flag (ADIF) */
    while(GET_BIT(ADC_u8_ADCSRA_REG, 4) == 0)
    {
        // Wait
    }

    /* 5. Clear ADIF flag by writing 1 to it (Hardware requirement) */
    SET_BIT(ADC_u8_ADCSRA_REG, 4);

    /* 6. Return the 10-bit result */
    return ADC_u16_ADC_REG;
}
