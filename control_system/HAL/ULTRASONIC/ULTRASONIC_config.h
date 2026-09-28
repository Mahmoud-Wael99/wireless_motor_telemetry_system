#ifndef ULTRASONIC_CONFIG_H
#define ULTRASONIC_CONFIG_H

/* Ultrasonic Pins based on main.c hardware setup (HC-SR04) */
#define ULTRASONIC_u8_TRIG_PORT    DIO_u8_PORTA
#define ULTRASONIC_u8_TRIG_PIN     DIO_u8_PIN0

#define ULTRASONIC_u8_ECHO_PORT    DIO_u8_PORTD
#define ULTRASONIC_u8_ECHO_PIN     DIO_u8_PIN6

/*
   Timeouts in microseconds (measured with hardware Timer0, so they do not
   depend on the CPU clock, compiler optimization or the DIO call overhead).
   - Echo start: the sensor normally raises Echo within ~1 ms of the trigger.
   - Echo width: the HC-SR04 gives a ~38 ms pulse when nothing is in range.
*/
#define ULTRASONIC_u32_ECHO_START_TIMEOUT_US   30000UL
#define ULTRASONIC_u32_ECHO_MAX_US             38000UL

/* Sound travels ~29 us per cm, and the echo pulse covers the round trip */
#define ULTRASONIC_u8_US_PER_CM                58

#endif
