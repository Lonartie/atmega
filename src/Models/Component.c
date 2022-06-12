#include "Component.h"

DEFINE_ACTOR_FORWARDER(void, Component, set_read);
DEFINE_ACTOR_FORWARDER(void, Component, set_write);
DEFINE_ACTOR_FORWARDER_N(void, Component, write, (bool value), (value));
DEFINE_ACTOR_FORWARDER(bool, Component, read);

Component Component_create(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t pin)
{
  Component comp;
  comp.ddr = ddr;
  comp.port = port;
  comp.pin = pin;

  SET_ACTOR_FORWARDER(comp, Component, set_read);
  SET_ACTOR_FORWARDER(comp, Component, set_write);
  SET_ACTOR_FORWARDER(comp, Component, write);
  SET_ACTOR_FORWARDER(comp, Component, read);

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