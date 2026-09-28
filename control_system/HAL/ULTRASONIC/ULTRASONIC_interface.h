#ifndef ULTRASONIC_INTERFACE_H
#define ULTRASONIC_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/*
   Sends a trigger pulse and measures the echo time.
   Returns distance in cm, or 0 if a timeout/error occurs.
*/
u16 ULTRASONIC_u16GetDistance(void);

#endif
