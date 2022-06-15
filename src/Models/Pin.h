#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdint.h>
#include <stdbool.h>

#include "../Misc/utils.h"
#include "../Misc/Vector.h"

/// @brief represents a pin on a microcontroller to have a high level 
///        abstraction to interact with it
typedef struct Pin
{
  volatile uint8_t* ddr;    ///< data direction register
  volatile uint8_t* port;   ///< port register
  uint8_t ddr_pin;          ///< ddr pin number
  uint8_t port_pin;         ///< port pin number

  /// @brief sets the pin to 'read'-mode (sensors)
  void(*set_read)();

  /// @brief sets the pin to 'write'-mode (actuators)
  void(*set_write)();

  /// @brief writes a value to the pin
  void(*write)(bool);

  /// @brief reads the value from the pin
  bool(*read)();
} Pin;

DECLARE_VECTORS(Pin);

/// @returns a new component (stack)
Pin Pin_create(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t ddr_pin, uint8_t port_pin);

///@brief sets the component to read mode 
void Pin_set_read(Pin* component);

///@brief sets the component to write mode
void Pin_set_write(Pin* component);

///@brief writes a value to the component
void Pin_write(Pin* component, bool value);

///@brief reads the value of the component
bool Pin_read(Pin* component);

#endif // COMPONENT_H