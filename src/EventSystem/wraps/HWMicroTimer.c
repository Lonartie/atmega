#include "HWMicroTimer.h"
#include "../libs/micros.h"

void hw_micros_timer_init()
{
  micros_init();
}
 
uint64_t hw_micros()
{
  return micros_get();
}