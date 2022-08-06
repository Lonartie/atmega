#ifndef SERIAL_H
#define SERIAL_H

#include <stdbool.h>
#include <stdint.h>

#include "Misc/String.h"

#define USART_EVENT_NAME "USART"

/// @brief a serial port
typedef struct USART {
  long ubrr;
  String event;
  char* volatile data;
} USART;

/// @returns the static singleton instance
USART* USART_instance();

/// @brief sends a single byte through the USART
/// @param usart the USART to send the byte through
/// @param byte the byte to send
void USART_send_byte(USART* usart, char byte);

/// @brief sends a string through the USART
/// @param usart the USART to send the string through
/// @param str the string to send
void USART_send_str(USART* usart, const char* str);

/// @brief receives a single byte through the USART
/// @param usart the USART to receive the byte through
/// @returns the received byte
char USART_recv_byte(USART* usart);

/// @brief starts the USART receiver to handle events
/// @param _this the USART to start the receiver for
void USART_start(USART* _this);

/// @brief stops the USART receiver
/// @param _this the USART to stop the receiver for
void USART_stop(USART* _this);

/// @brief updates the USART receiver and actually sends data
/// @param _this the USART to update the receiver for
void USART_update(void* _this);

#endif  // SERIAL_H