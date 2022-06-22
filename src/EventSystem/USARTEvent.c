#include "USARTEvent.h"
#include "../Misc/Debug.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h> 

ISR(USART_RX_vect,ISR_BLOCK)
{
  debug("USART event? help!");
}