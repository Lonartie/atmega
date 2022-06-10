#include "HardwareTimer.h"
#include "libs/millis.h"

void timer_init()
{
  millis_init();
}

uint64_t millis()
{
  return millis_get();
}