#ifndef ENCODER_CONFIG_H
#define ENCODER_CONFIG_H

/*
   Phase A must be connected to INT0 = PD2 on the ATmega32.
   EXTI0_u8_SENSE_CONTROL must be EXTI_u8_RISING_EDGE (see EXTI_config.h),
   because the direction logic samples Phase B on Phase A's rising edge.
*/
#define ENCODER_u8_PHASE_A_PORT   DIO_u8_PORTD
#define ENCODER_u8_PHASE_A_PIN    DIO_u8_PIN2

/*
   Phase B pin used for direction sensing.
   Pin map of this project (keep them all different!):
       PC0 = motor DIR1     PC1 = motor DIR2     PC2 = encoder Phase B
       PC3 = I2C SCL        PC4 = I2C SDA        PD2 = encoder Phase A (INT0)
   PC2..PC5 are JTAG pins on the ATmega32: main.c disables JTAG at start-up
   (MCUCSR.JTD) so they work as normal I/O. You can also unprogram the JTAGEN
   fuse. If you ever move Phase B, pick a pin that no other config uses.
*/
#define ENCODER_u8_PHASE_B_PORT   DIO_u8_PORTC
#define ENCODER_u8_PHASE_B_PIN    DIO_u8_PIN2

/*
   Internal pull-up on the encoder inputs:
   1 = enable (use for open-collector / Hall encoder outputs)
   0 = disable (use if the encoder or an external resistor drives the line)
*/
#define ENCODER_u8_PULLUP_ENABLE  1

/*
   Pulses Per Revolution (PPR) of the output shaft, counting Phase A
   rising edges only.
   25GA370 (210 RPM version): 11 pulses/rev on the motor shaft x 34:1 gear
   ratio = 374. Adjust if your gear ratio or magnetic disc is different.
*/
#define ENCODER_u16_PPR           374

#endif
