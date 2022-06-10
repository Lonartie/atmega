#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  long ubrr;
  bool heap;    //< whether or not the object is allocated on the heap
} USART;

/// @returns a new USART (stack)
USART USART_create();

/// @returns a new USART (heap / malloc)
USART* USART_new();

/// @brief destroys the USART
void USART_destroy(USART* usart);

/// @brief sends a single byte through the USART
void USART_send_byte(USART* usart, char byte);

/// @brief sends a string through the USART
void USART_send_str(USART* usart, const char* str);

/// @brief receives a single byte through the USART
/// @returns the received byte
uint8_t USART_recv_byte(USART* usart);


#endif // SERIAL_H