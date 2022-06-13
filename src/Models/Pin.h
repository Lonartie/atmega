#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../Misc/Actor.h"

typedef struct Pin
{
  volatile uint8_t* ddr;    ///< data direction register
  volatile uint8_t* port;   ///< port register
  uint8_t pin;              ///< pin number

  void(*set_read)();
  void(*set_write)();
  void(*write)(bool);
  bool(*read)();
} Pin;

/// @returns a new component (stack)
Pin Pin_create(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t pin_num);

///@brief sets the component to read mode 
void Pin_set_read(Pin* component);

///@brief sets the component to write mode
void Pin_set_write(Pin* component);

///@brief writes a value to the component
void Pin_write(Pin* component, bool value);

///@brief reads the value of the component
bool Pin_read(Pin* component);

#endif // COMPONENT_H