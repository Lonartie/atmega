#include "Component.h"

Component Component_create(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t pin)
{
  Component comp;
  comp.ddr = ddr;
  comp.port = port;
  comp.pin = pin;

  comp.set_read = Component_set_read;
  comp.set_write = Component_set_write;
  comp.write = Component_write;
  comp.read = Component_read;

  return comp;
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