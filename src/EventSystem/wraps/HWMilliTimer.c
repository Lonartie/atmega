#include "HWMicroTimer.h"
#include "../libs/millis.h"

void hw_millis_timer_init()
{
  millis_init();
}
 
uint64_t hw_millis()
{
  return millis_get();
}