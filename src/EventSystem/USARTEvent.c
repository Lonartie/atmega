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
  char msg[32];
  uint8_t i = 0;
  while (1) {
    msg[i] = USART_recv_byte(&usart);
    if (msg[i] == '\r') {
      break;
    }
    ++i;
  }
  msg[i] = '\0';
  debug(msg);
}