#include "System.h"
#include "Misc/utils.h"
#include <avr/io.h>

System System_create()
{
  System system;
  system.led_strip = ShiftRegister_create(&DDRD, &PORTD, DDD4, PORTD4, &DDRB, &PORTB, DDB2, PORTB2, 3, 1);
  system.logger = USART_create();
  system.lf_left = Pin_create(&DDRC, &PINC, DDC0, PINC0);
  system.lf_middle = Pin_create(&DDRC, &PINC, DDC1, PINC1);
  system.lf_right = Pin_create(&DDRC, &PINC, DDC2, PINC2);

  ShiftRegister_clear(&system.led_strip);
  ACTOR(system.lf_left).set_read();
  ACTOR(system.lf_middle).set_read();
  ACTOR(system.lf_right).set_read();

  return system;
}