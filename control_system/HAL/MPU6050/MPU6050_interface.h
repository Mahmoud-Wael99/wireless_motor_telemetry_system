#ifndef MPU6050_INTERFACE_H
#define MPU6050_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/* Initializes the Software I2C bus and wakes up the MPU6050 */
void MPU6050_voidInit(void);

/*
   Reads the raw X, Y, and Z acceleration values.
   Pass the addresses of three s16 variables to hold the data.
*/
void MPU6050_voidGetAccelRaw(s16* Copy_ps16AccelX, s16* Copy_ps16AccelY, s16* Copy_ps16AccelZ);

#endif
