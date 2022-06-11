#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../Misc/Actor.h"

typedef struct Component
{
  volatile uint8_t* ddr;    ///< data direction register
  volatile uint8_t* port;   ///< port register
  uint8_t pin;              ///< pin number

  void(*set_read)();
  void(*set_write)();
  void(*write)(bool);
  bool(*read)();
  DECLARE_ACTOR_MEM(Component);
} Component;

DECLARE_ACTOR(Component);

/// @returns a new component (stack)
Component Component_create(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t pin);

///@brief sets the component to read mode 
void Component_set_read(Component* component);

///@brief sets the component to write mode
void Component_set_write(Component* component);

///@brief writes a value to the component
void Component_write(Component* component, bool value);

///@brief reads the value of the component
bool Component_read(Component* component);

#endif // COMPONENT_H