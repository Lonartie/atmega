#include "USARTEvent.h"
#include "../Misc/Debug.h"
#include "../Models/USART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h> 

static USART usart;
static bool initialized = false;

USARTEvent USARTEvent_create()
{
  if (!initialized) {
    usart = USART_create();
    initialized = true;
  }

  USARTEvent event;
  return event;
}

ISR(USART_RX_vect)
{
  char data[64];
  uint8_t i = 0;
  do {
    data[i] = USART_recv_byte(&usart);
  } while(data[i++] != '\r');

  debug(data);
}