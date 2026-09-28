#ifndef MPU6050_CONFIG_H
#define MPU6050_CONFIG_H

/*
   Base Address: 0x68  (AD0 pin low; use 0x69 if AD0 is tied high)
   Write Address: (0x68 << 1) | 0 = 0xD0
   Read Address:  (0x68 << 1) | 1 = 0xD1
*/
#define MPU6050_u8_ADDRESS_WRITE    0xD0
#define MPU6050_u8_ADDRESS_READ     0xD1

/* The MPU6050 needs up to ~100 ms after power-up before it answers on I2C */
#define MPU6050_u8_POWERUP_DELAY_MS 100

/* How many times to retry the wake-up command if the chip does not ACK */
#define MPU6050_u8_INIT_RETRIES     3

#endif
