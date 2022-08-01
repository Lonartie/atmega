#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdbool.h>
#include <stdint.h>

#include "Misc/Utils.h"
#include "Misc/Vector.h"

/// @brief represents a pin on a microcontroller to have a high level
///        abstraction to interact with it
typedef struct Pin {
  volatile uint8_t* ddr;   // data direction register
  volatile uint8_t* port;  // port register
  uint8_t ddr_pin;         // ddr pin number
  uint8_t port_pin;        // port pin number
} Pin;

DECLARE_VECTORS(Pin);

/// @returns a new pin (stack)
Pin Pin_create(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t ddr_pin,
               uint8_t port_pin);

///@brief sets the pin to read mode
void Pin_set_read(Pin* pin);

///@brief sets the pin to write mode
void Pin_set_write(Pin* pin);

///@brief writes a value to the pin
void Pin_write(Pin* pin, bool value);

///@brief reads the value of the pin
bool Pin_read(Pin* pin);

#endif  // COMPONENT_H