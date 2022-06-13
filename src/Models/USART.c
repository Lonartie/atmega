#define BAUD 9600
#ifndef F_CPU
#define F_CPU 16E6
#endif
#define UBRR_SETTING F_CPU/16.0/BAUD-1
#ifndef __AVR_ATmega328P__
#	define __AVR_ATmega328P__
#endif

#include "USART.h"
#include <avr/io.h>

DEFINE_ACTOR_FORWARDER_N(void, USART, send_byte, (char value), (value));
DEFINE_ACTOR_FORWARDER_N(void, USART, send_str, (const char* str), (str));
DEFINE_ACTOR_FORWARDER(char, USART, recv_byte);

USART USART_create()
{
  USART usart;
  usart.ubrr = UBRR_SETTING;
  usart.heap = false;

  SET_ACTOR_FORWARDER(usart, USART, send_byte);
  SET_ACTOR_FORWARDER(usart, USART, send_str);
  SET_ACTOR_FORWARDER(usart, USART, recv_byte);

  UBRR0H = (unsigned char) (usart.ubrr >> 8);
  UBRR0L = (unsigned char) usart.ubrr;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
  USART_send_str(&usart, "<(^_^)>\n\0");

  return usart;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void USART_send_byte(USART* usart, char byte)
{
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = byte;
}
#pragma GCC diagnostic pop

void USART_send_str(USART* usart, const char* str)
{
  while (*str != '\0') {
    USART_send_byte(usart, *str);
    str++;
  }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
char USART_recv_byte(USART* usart)
{
  while (!(UCSR0A & (1 << RXC0)));
  return UDR0;
}
#pragma GCC diagnostic pop