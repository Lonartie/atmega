#define BAUD 9600
#define UBRR_SETTING F_CPU / 16.0 / BAUD - 1

#include "USART.h"

#include <avr/io.h>

#include "Misc/Utils.h"

static USART instance;
static bool initialized = false;

USART* USART_instance() {
  if (!initialized) {
    instance.ubrr = UBRR_SETTING;
    instance.heap = false;

    UBRR0H = (unsigned char)(instance.ubrr >> 8);
    UBRR0L = (unsigned char)instance.ubrr;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
    initialized = true;
  }

  return &instance;
}

void USART_send_byte(USART* usart MAYBE_UNUSED, char byte) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = byte;
}

void USART_send_str(USART* usart, const char* str) {
  while (*str != '\0') {
    USART_send_byte(usart, *str);
    str++;
  }
}

char USART_recv_byte(USART* usart MAYBE_UNUSED) {
  while (!(UCSR0A & (1 << RXC0)))
    ;
  return UDR0;
}