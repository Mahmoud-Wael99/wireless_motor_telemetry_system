#ifndef SENSORS_CONFIG_H
#define SENSORS_CONFIG_H

/* ADC Channels based on main.c hardware setup */
#define SENSORS_u8_NTC_CHANNEL       2  /* PA2 */
#define SENSORS_u8_VOLTAGE_CHANNEL   4  /* PA4 */

/* Voltage Sensor Resistor Divider Values (Standard 25V Module) */
#define SENSORS_f32_R1               30000.0
#define SENSORS_f32_R2               7500.0

/* NTC 10K Thermistor Parameters.
   The formula in SENSORS_program.c assumes this wiring:
       VCC -- 10k series resistor --+-- NTC -- GND      (ADC pin at the "+")
   If your NTC is on the VCC side instead, the temperature will read inverted. */
#define SENSORS_u16_SERIES_RESISTOR  10000
#define SENSORS_u16_NOMINAL_RESIST   10000
#define SENSORS_u8_NOMINAL_TEMP      25
#define SENSORS_u16_B_COEFFICIENT    3950

#endif
