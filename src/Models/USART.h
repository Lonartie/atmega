#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>
#include "../Misc/Actor.h"

/// @brief a serial port
typedef struct USART {
  long ubrr;
  bool heap;    //< whether or not the object is allocated on the heap

  void(*destroy)();
  void (*send_byte)(char);
  void (*send_str)(const char*);
  char (*recv_byte)();
} USART;

/// @returns a new USART (stack)
USART USART_create();

/// @brief sends a single byte through the USART
void USART_send_byte(USART* usart, char byte);

/// @brief sends a string through the USART
void USART_send_str(USART* usart, const char* str);

/// @brief receives a single byte through the USART
/// @returns the received byte
char USART_recv_byte(USART* usart);

#endif // SERIAL_H