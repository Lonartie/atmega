#include "HardwareTimer.h"
#include "wraps/HWMicroTimer.h"
#include "wraps/HWMilliTimer.h"

void timer_init()
{
  hw_micros_timer_init();
  hw_millis_timer_init();
}

uint64_t millis()
{
  return hw_millis();
}
 
uint64_t micros()
{
  return hw_micros();
}