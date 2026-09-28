#ifndef CLOCK_H_
#define CLOCK_H_

/*
   Single source of truth for the CPU clock.
   The Eclipse project passes -DF_CPU=8000000UL (Project > Properties > AVR >
   Target Hardware). This is only a fallback with the SAME value so that no
   file ever silently assumes a different clock.
   If you change the crystal / fuses, change the project setting too.
*/
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#endif
