#include "System.h"
#include "utils.h"

System System_create()
{
  System system;
  system.led_strip = ShiftRegister_create(&DDRD, &PORTD, DDD4, &DDRB, &PORTB, DDB2, 3, 1);
  system.logger = USART_create();

  ShiftRegister_clear(&system.led_strip);
  return system;
}