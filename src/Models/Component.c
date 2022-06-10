#include "Component.h"

Component Component_create(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t pin)
{
  Component comp;
  comp.ddr = ddr;
  comp.port = port;
  comp.pin = pin;
  comp.heap = false;
  return comp;
}

Component* Component_new(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t pin)
{
  Component* comp = malloc(sizeof(Component) * 1);
  comp->ddr = ddr;
  comp->port = port;
  comp->pin = pin;
  comp->heap = true;
  return comp;
}

void Component_destroy(Component* component)
{
  if (component->heap)
    free(component);
}

void Component_set_read(Component* component)
{
  (*component->ddr) &= ~(1 << component->pin);
}

void Component_set_write(Component* component)
{
  (*component->ddr) |= (1 << component->pin);
}

void Component_write(Component* component, bool value)
{
  if (value)  (*component->port) |=  (1 << component->pin);
  else        (*component->port) &= ~(1 << component->pin);
}

bool Component_read(Component* component)
{
  return (*component->port) & (1 << component->pin);
}