#include "delay.h"
#include <unistd.h>

void _delay_us(unsigned int us)
{
  usleep(us);
}

void _delay_ms(unsigned int ms)
{
  usleep(ms * 1000);
}