#include "USARTEvent.h"
#include "../Misc/Debug.h"
#include "../Models/USART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

static USART usart;
static ShiftRegister leds;
static bool initialized = false;

USARTEvent USARTEvent_create(ShiftRegister led)
{
  if (!initialized) {
    usart = USART_create();
    leds = led;
    initialized = true;
  }

  USARTEvent event;
  return event;
}

ISR(USART_RX_vect)
{
  static bool a = true, b = false, c = false;
  bool tmp = c;
  c = b;
  b = a;
  a = tmp;
  ShiftRegister_write_n(&leds, a, b, c);
}