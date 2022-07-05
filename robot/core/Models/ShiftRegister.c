#include "ShiftRegister.h"

#include <stdarg.h>
#include <util/delay.h>

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

  Pin_set_write(&sr.clk);
  Pin_set_write(&sr.data);
  return sr;
}

void ShiftRegister_clear(ShiftRegister* sr) 
{
  for (uint16_t i = 0; i < sr->size; i++)
    ShiftRegister_write(sr, false);
}

void ShiftRegister_write(ShiftRegister* sr, bool value)
{
  Pin_write(&sr->clk, 0);
  Pin_write(&sr->data, value);
  Pin_write(&sr->clk, 1);
  
  for (uint8_t i = 0; i < sr->tpd_mcs; i++)
    _delay_us(1); // because _delay_us expects a compile time constant
  
  Pin_write(&sr->clk, 0);
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