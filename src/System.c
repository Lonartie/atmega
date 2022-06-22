#include "System.h"
#include "Misc/utils.h"
#include <avr/io.h>

System System_create()
{
  System system;
  system.led_strip = ShiftRegister_create(&DDRD, &PORTD, DDD4, PORTD4, &DDRB, &PORTB, DDB2, PORTB2, 3, 1);
  system.logger = USART_create();
  system.lf_left = Pin_create(&DDRC, &PINC, DDC2, PINC2);
  system.lf_middle = Pin_create(&DDRC, &PINC, DDC1, PINC1);
  system.lf_right = Pin_create(&DDRC, &PINC, DDC0, PINC0);
  system.mt_left = Motor_create(PD5, Pin_create(&DDRD, &PORTD, PD7, PORTD7), Pin_create(&DDRB, &PORTB, PB0, PORTB0));
  system.mt_right = Motor_create(PD6, Pin_create(&DDRB, &PORTB, PB3, PORTB3), Pin_create(&DDRB, &PORTB, PB1, PORTB1));

  ShiftRegister_clear(&system.led_strip);
  Pin_set_read(&system.lf_left);
  Pin_set_read(&system.lf_middle);
  Pin_set_read(&system.lf_right);

  return system;
}