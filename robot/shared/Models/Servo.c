#include "Servo.h"
#include "Misc/Utils.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

static bool timer_2_initialized = false;
void init_timer_2();

Servo Servo_create(Pin pin, volatile uint8_t* reg, uint8_t delay_ms)
{
  Servo servo;
  servo.pin = pin;
  servo.reg = reg;
  servo.delay_ms = delay_ms;

  Pin_set_write(&servo.pin);

  if (!timer_2_initialized)
  {
    init_timer_2();
    timer_2_initialized = true;
  }

  return servo;
}

void Servo_set_angle(Servo* _this, char angle)
{
  if (angle > 90 || angle < -90)
  {
    return;
  }

  unsigned char value = 0;

  if (angle >= 0) 
  {
    value = (unsigned char) ((float) angle / 90.0f * 19.0f + 21.0f);
  } 
  else 
  {
    value = (unsigned char) ((float) angle / 90.0f * 14.0f + 21.0f);
  }

  *(_this->reg) = value;

  for (int i = 0; i < _this->delay_ms; i++) 
  {
    _delay_ms(1);
  }
}

void init_timer_2()
{
  TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
}