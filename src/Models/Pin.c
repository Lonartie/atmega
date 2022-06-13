#include "Pin.h"

DEFINE_ACTOR_FORWARDER(void, Pin, set_read);
DEFINE_ACTOR_FORWARDER(void, Pin, set_write);
DEFINE_ACTOR_FORWARDER_N(void, Pin, write, (bool value), (value));
DEFINE_ACTOR_FORWARDER(bool, Pin, read);

Pin Pin_create(volatile uint8_t* ddr, volatile uint8_t* port, uint8_t pin_num)
{
  Pin pin;
  pin.ddr = ddr;
  pin.port = port;
  pin.pin = pin_num;

  SET_ACTOR_FORWARDER(pin, Pin, set_read);
  SET_ACTOR_FORWARDER(pin, Pin, set_write);
  SET_ACTOR_FORWARDER(pin, Pin, write);
  SET_ACTOR_FORWARDER(pin, Pin, read);

  return pin;
}

void Pin_set_read(Pin* pin)
{
  (*pin->ddr) &= ~(1 << pin->pin);
}

void Pin_set_write(Pin* pin)
{
  (*pin->ddr) |= (1 << pin->pin);
}

void Pin_write(Pin* pin, bool value)
{
  if (value)  (*pin->port) |=  (1 << pin->pin);
  else        (*pin->port) &= ~(1 << pin->pin);
}

bool Pin_read(Pin* pin)
{
  return (*pin->port) & (1 << pin->pin);
}