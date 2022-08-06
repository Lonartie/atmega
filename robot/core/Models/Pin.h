#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdbool.h>
#include <stdint.h>

#include "Misc/Utils.h"
#include "Misc/Vector.h"

/// @brief represents a pin on a microcontroller
typedef struct Pin {
  volatile uint8_t* ddr;   // data direction register
  volatile uint8_t* port;  // port register
  uint8_t ddr_pin;         // ddr pin number
  uint8_t port_pin;        // port pin number
} Pin;

DECLARE_VECTORS(Pin);

/// @brief creates a new pin
/// @param ddr the data direction register
/// @param port the port register
/// @param ddr_pin the ddr pin number
/// @param port_pin the port pin number
/// @returns a new pin
Pin Pin_create(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t ddr_pin,
               uint8_t port_pin);

///@brief sets the pin to read mode
///@param pin the pin to set
void Pin_set_read(Pin* pin);

///@brief sets the pin to write mode
///@param pin the pin to set
void Pin_set_write(Pin* pin);

///@brief writes a value to the pin
///@param pin the pin to write to
///@param value the value to write to the pin
void Pin_write(Pin* pin, bool value);

///@brief reads the value of the pin
///@param pin the pin to read from
///@returns the value of the pin
bool Pin_read(Pin* pin);

#endif  // COMPONENT_H