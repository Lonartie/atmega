#ifndef HARDWARE_TIMER_H
#define HARDWARE_TIMER_H

#include <stdint.h>

void timer_init();
uint64_t millis(); 
uint64_t micros();

#endif // HARDWARE_TIMER_H