#include "../LIB/CLOCK.h"      /* F_CPU (single source of truth) - must come before delay.h */
#include <avr/io.h>            /* MCUCSR / JTD for the JTAG disable sequence */
#include <util/delay.h>
#include <stdio.h> /* Included for sprintf to easily format telemetry strings */
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

/* MCAL Layer Inclusions */
#include "../MCAL/DIO/DIO_interface.h"
#include "../MCAL/GIE/GIE_interface.h"
#include "../MCAL/ADC/ADC_interface.h"
#include "../MCAL/UART/UART_interface.h"

/* HAL Layer Inclusions */
#include "../HAL/MOTOR/MOTOR_interface.h"
#include "../HAL/ULTRASONIC/ULTRASONIC_interface.h"
#include "../HAL/ENCODER/ENCODER_interface.h"
#include "../HAL/SENSORS/SENSORS_interface.h"
#include "../HAL/BUZZER/BUZZER_interface.h"
#include "../HAL/MPU6050/MPU6050_interface.h"

/*
   The ATmega32 ships with the JTAG interface enabled (JTAGEN fuse), which takes
   over PC2 (TCK), PC3 (TMS), PC4 (TDO) and PC5 (TDI). This project uses PC2
   (encoder Phase B), PC3 (I2C SCL) and PC4 (I2C SDA), so JTAG must be off.
   Setting MCUCSR.JTD requires a timed sequence: write the bit twice within 4
   clock cycles. Two back-to-back OUT instructions guarantee that.
   (Programming through ISP/avrdude is not affected. If you debug over JTAG,
   remove this call.)
*/
static void APP_voidDisableJTAG(void)
{
    u8 Local_u8Value = MCUCSR | (1 << JTD);

    __asm__ __volatile__(
        "out %[reg], %[val]\n\t"
        "out %[reg], %[val]\n\t"
        :
        : [reg] "I" (_SFR_IO_ADDR(MCUCSR)), [val] "r" (Local_u8Value)
    );
}

int main(void)
{
    /* Free PC2..PC5 (JTAG pins) before they are used as normal I/O */
    APP_voidDisableJTAG();

    /* ========================================================= */
    /* 1. Pin Direction Initialization                           */
    /* ========================================================= */

    /* Motor Driver Pins */
    DIO_voidSetPinDirection(DIO_u8_PORTC, DIO_u8_PIN0, DIO_u8_INITIAL_OUTPUT); // DIR1
    DIO_voidSetPinDirection(DIO_u8_PORTC, DIO_u8_PIN1, DIO_u8_INITIAL_OUTPUT); // DIR2
    DIO_voidSetPinDirection(DIO_u8_PORTD, DIO_u8_PIN5, DIO_u8_INITIAL_OUTPUT); // PWM (Timer1 OC1A)

    /* Encoder Pins */
    DIO_voidSetPinDirection(DIO_u8_PORTD, DIO_u8_PIN2, DIO_u8_INITIAL_INPUT);  // Phase A (INT0)
    DIO_voidSetPinDirection(DIO_u8_PORTC, DIO_u8_PIN2, DIO_u8_INITIAL_INPUT);  // Phase B
    DIO_voidSetPinValue(DIO_u8_PORTD, DIO_u8_PIN2, DIO_u8_HIGH);               // Internal Pull-up for Phase A

    /* Safety & Sensors Pins */
    DIO_voidSetPinDirection(DIO_u8_PORTA, DIO_u8_PIN0, DIO_u8_INITIAL_OUTPUT); // Ultrasonic Trig
    DIO_voidSetPinDirection(DIO_u8_PORTD, DIO_u8_PIN6, DIO_u8_INITIAL_INPUT);  // Ultrasonic Echo
    DIO_voidSetPinDirection(DIO_u8_PORTA, DIO_u8_PIN1, DIO_u8_INITIAL_OUTPUT); // Buzzer
    DIO_voidSetPinDirection(DIO_u8_PORTA, DIO_u8_PIN2, DIO_u8_INITIAL_INPUT);  // NTC Thermistor (ADC2)
    DIO_voidSetPinDirection(DIO_u8_PORTA, DIO_u8_PIN4, DIO_u8_INITIAL_INPUT);  // Voltage Sensor (ADC4)

    /* UART Telemetry Pins */
    DIO_voidSetPinDirection(DIO_u8_PORTD, DIO_u8_PIN1, DIO_u8_INITIAL_OUTPUT); // ATmega TX
    DIO_voidSetPinDirection(DIO_u8_PORTD, DIO_u8_PIN0, DIO_u8_INITIAL_INPUT);  // ATmega RX

    /* Software I2C Pins (MPU6050): PC3 = SCL, PC4 = SDA.
       Configured by I2C_SW_voidInit() as open-drain lines, so nothing to do here
       (forcing them to outputs here would pull both lines LOW until init). */

    /* ========================================================= */
    /* 2. Peripheral Initialization                              */
    /* ========================================================= */

    ADC_voidInit();
    UART_voidInit();

    /* Boot message: if you see this on the ESP32 dashboard, the UART link
       (wiring, level shifter and baud rate) is working. */
    UART_voidSendString("ATmega32 control system started\r\n");

    MOTOR_voidInit();
    ENCODER_voidInit();
    MPU6050_voidInit();

    /* Enable Global Interrupts after all configurations are set */
    GIE_voidEnable();

    /* Application Variables */
    u16 Local_u16Distance = 0;
    u16 Local_u16Millivolts = 0;
    u8 Local_u8Temp = 0;
    u32 Local_u32Pulses = 0;
    s16 Local_s16AccelX, Local_s16AccelY, Local_s16AccelZ;

    char TelemetryBuffer[100];
    u8 TelemetryCounter = 0;

    /* ========================================================= */
    /* 3. Application Super Loop                                 */
    /* ========================================================= */
    while(1)
    {
        /* A. Safety Check: Read Ultrasonic Distance */
        Local_u16Distance = ULTRASONIC_u16GetDistance();

        /* If an object is closer than 15 cm, execute emergency stop */
        if(Local_u16Distance > 0 && Local_u16Distance < 15)
        {
            MOTOR_voidStop();
            BUZZER_voidOn();
            UART_voidSendString("WARNING: Obstacle Detected!\r\n");

            _delay_ms(500);
        }
        else
        {
            /* Path is clear: Run motor and ensure buzzer is off */
            BUZZER_voidOff();

            MOTOR_voidSetDirection(MOTOR_u8_CW);
            MOTOR_voidSetSpeed(150);

            /* Increment loop counter to manage telemetry rate */
            TelemetryCounter++;

            /* B. Telemetry Broadcast Check (~Every few loops to avoid UART spam) */
            if(TelemetryCounter >= 20)
            {
                /* Read all sensors */
                Local_u32Pulses = ENCODER_u32GetPulseCount();
                Local_u16Millivolts = SENSORS_u16GetMillivolts();
                Local_u8Temp = SENSORS_u8GetTemperature();
                MPU6050_voidGetAccelRaw(&Local_s16AccelX, &Local_s16AccelY, &Local_s16AccelZ);

                /* Format the data string into the buffer */
                sprintf(TelemetryBuffer,
                        "P:%lu, V:%u mV, T:%u C, X:%d, Y:%d, Z:%d\r\n",
                        Local_u32Pulses,
                        Local_u16Millivolts,
                        Local_u8Temp,
                        Local_s16AccelX,
                        Local_s16AccelY,
                        Local_s16AccelZ);

                /* Send over the ESP32 Gateway link */
                UART_voidSendString(TelemetryBuffer);

                TelemetryCounter = 0;
            }

            /* A short delay to stabilize the loop timing */
            _delay_ms(50);
        }
    }

    return 0;
}
