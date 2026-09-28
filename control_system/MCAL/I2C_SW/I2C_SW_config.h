#ifndef I2C_SW_CONFIG_H
#define I2C_SW_CONFIG_H

/* Software I2C Pins Mapping
   NOTE: PC2..PC5 are JTAG pins on the ATmega32. main.c disables JTAG at
   start-up (MCUCSR.JTD), or you can unprogram the JTAGEN fuse. */
#define I2C_SW_u8_SCL_PORT      DIO_u8_PORTC
#define I2C_SW_u8_SCL_PIN       DIO_u8_PIN3

#define I2C_SW_u8_SDA_PORT      DIO_u8_PORTC
#define I2C_SW_u8_SDA_PIN       DIO_u8_PIN4

/* Half-clock delay in microseconds (approx 100kHz standard mode at best;
   the DIO function calls make the real bus a bit slower, which is fine) */
#define I2C_SW_u8_DELAY_US      5

/*
   Line driving mode:
   1 = OPEN-DRAIN (recommended, real I2C behaviour): the MCU only ever pulls a
       line LOW or releases it (input, no internal pull-up). The lines are
       pulled HIGH by the pull-up resistors on the MPU6050 (GY-521) board.
       Never pushes 5 V into the 3.3 V sensor and never fights the slave.
   0 = PUSH-PULL: the MCU actively drives HIGH. Only use this if your module
       has NO pull-up resistors at all.
*/
#define I2C_SW_u8_OPEN_DRAIN    1

#endif
