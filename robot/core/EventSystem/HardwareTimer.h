#ifndef HARDWARE_TIMER_H
#define HARDWARE_TIMER_H

#include <stdint.h>

/// @brief initializes the hardware timer using the interrupts TIMER0_COMPA_vect
/// and TIMER1_COMPA_vect
void timer_init();

/// @returns the current time since init in milliseconds
uint64_t millis();

/// @returns the current time since init in microseconds
uint64_t micros();

#endif  // HARDWARE_TIMER_H