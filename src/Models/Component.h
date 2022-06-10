#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
  volatile uint8_t* ddr;    ///< data direction register
  volatile uint8_t* port;   ///< port register
  uint8_t pin;              ///< pin number
  bool heap;                ///< whether the component is allocated on the heap
} Component;

/// @returns a new component (stack)
Component Component_create(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t pin);

/// @returns a new component (heap / malloc)
Component* Component_new(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t pin);

/// @brief frees the component if it was alloced on the heap
void Component_destroy(Component* component);

///@brief sets the component to read mode 
void Component_set_read(Component* component);

///@brief sets the component to write mode
void Component_set_write(Component* component);

///@brief writes a value to the component
void Component_write(Component* component, bool value);

///@brief reads the value of the component
bool Component_read(Component* component);

#endif // COMPONENT_H