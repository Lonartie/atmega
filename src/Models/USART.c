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
#include <stdlib.h>

USART USART_create()
{
  USART usart;
  usart.ubrr = UBRR_SETTING;
  usart.heap = false;

  UBRR0H = (unsigned char) (usart.ubrr >> 8);
  UBRR0L = (unsigned char) usart.ubrr;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
  USART_send_str(&usart, "<(^_^)>\n\0");

  return usart;
}

USART* USART_new()
{
  USART* usart = malloc(sizeof(USART));
  usart->ubrr = UBRR_SETTING;
  usart->heap = true;

  UBRR0H = (unsigned char) (usart->ubrr >> 8);
  UBRR0L = (unsigned char) usart->ubrr;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
  USART_send_str(usart, "<(^_^)>\n\0");

  return usart;
}

void USART_destroy(USART* usart)
{
  if (usart->heap)
    free(usart);
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
uint8_t USART_recv_byte(USART* usart)
{
  while (!(UCSR0A & (1 << RXC0)));
  return UDR0;
}
#pragma GCC diagnostic pop