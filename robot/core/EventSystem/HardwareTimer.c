#include "HardwareTimer.h"

#include "wraps/HWMicroTimer.h"

void timer_init() { hw_micros_timer_init(); }

uint64_t millis() { return hw_micros() / 1000; }

uint64_t micros() { return hw_micros(); }