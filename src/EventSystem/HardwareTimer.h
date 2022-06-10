#ifndef HARDWARE_TIMER_H
#define HARDWARE_TIMER_H

#include <stdint.h>

// Initializes the use of the timer functions
void initTimer();

// Gets the milliseconds of the current time.
uint32_t millis();

#endif // HARDWARE_TIMER_H