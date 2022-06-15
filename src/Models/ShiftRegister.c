#include "ShiftRegister.h"

#include <stdarg.h>
#include <util/delay.h>

DEFINE_ACTOR_FORWARDER(void, ShiftRegister, clear);
DEFINE_ACTOR_FORWARDER_N(void, ShiftRegister, write, (bool value), (value));
DECLARE_ACTOR_FORWARDER_N(void, ShiftRegister, write_n, (uint32_t n, ...));

ShiftRegister ShiftRegister_create(
  volatile uint8_t* clk_ddr, volatile uint8_t* clk_port, uint8_t clk_ddr_pin, uint8_t clk_port_pin,
  volatile uint8_t* data_ddr, volatile uint8_t* data_port, uint8_t data_ddr_pin, uint8_t data_port_pin,
  uint16_t size, uint16_t tpd_mcs)
{
  ShiftRegister sr;
  sr.clk = Pin_create(clk_ddr, clk_port, clk_ddr_pin, clk_port_pin);
  sr.data = Pin_create(data_ddr, data_port, data_ddr_pin, data_port_pin);
  sr.size = size;
  sr.tpd_mcs = tpd_mcs;
  
  SET_ACTOR_FORWARDER(sr, ShiftRegister, clear);
  SET_ACTOR_FORWARDER(sr, ShiftRegister, write);
  SET_ACTOR_FORWARDER(sr, ShiftRegister, write_n);

  ACTOR(sr.clk).set_write();
  ACTOR(sr.data).set_write();
  return sr;
}

void ShiftRegister_clear(ShiftRegister* sr)
{
  for (uint16_t i = 0; i < sr->size; i++)
    ShiftRegister_write(sr, false);
}

void ShiftRegister_write(ShiftRegister* sr, bool value)
{
  ACTOR(sr->clk).write(0);
  ACTOR(sr->data).write(value);
  ACTOR(sr->clk).write(1);
  
  for (uint8_t i = 0; i < sr->tpd_mcs; i++)
    _delay_us(1); // because _delay_us expects a compile time constant
  
  ACTOR(sr->clk).write(0);
}

void ShiftRegister_write_n(ShiftRegister* sr, uint32_t n, ...)
{
  va_list args;
  va_start(args, n);
  for (uint16_t i = 0; i < n; i++)
  {
    bool value = va_arg(args, int); // bool gets promoted to int through va_args
    ShiftRegister_write(sr, value);
  }
  va_end(args);
}

void ShiftRegister_write_n_actor(uint32_t n, ...)
{
  va_list args;
  va_start(args, n);
  for (uint16_t i = 0; i < n; i++)
  {
    bool value = va_arg(args, int); // bool gets promoted to int through va_args
    ShiftRegister_write_actor(value);
  }
  va_end(args);
}