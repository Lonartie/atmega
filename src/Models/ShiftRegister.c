#include "ShiftRegister.h"

#include <stdarg.h>
#include <util/delay.h>

ShiftRegister ShiftRegister_create(
  volatile uint8_t* clk_ddr, volatile uint8_t* clk_port, uint8_t clk_pin,
  volatile uint8_t* data_ddr, volatile uint8_t* data_port, uint8_t data_pin,
  uint16_t size, uint16_t tpd_mcs)
{
  ShiftRegister shift_register;
  shift_register.clk = Component_create(clk_ddr, clk_port, clk_pin);
  shift_register.data = Component_create(data_ddr, data_port, data_pin);
  shift_register.size = size;
  shift_register.tpd_mcs = tpd_mcs;
  return shift_register;
}

ShiftRegister* ShiftRegister_new(
  volatile uint8_t* clk_ddr, volatile uint8_t* clk_port, uint8_t clk_pin,
  volatile uint8_t* data_ddr, volatile uint8_t* data_port, uint8_t data_pin,
  uint16_t size, uint16_t tpd_mcs)
{
  ShiftRegister* shift_register = malloc(sizeof(ShiftRegister) * 1);
  shift_register->clk = Component_create(clk_ddr, clk_port, clk_pin);
  shift_register->data = Component_create(data_ddr, data_port, data_pin);
  shift_register->size = size;
  shift_register->tpd_mcs = tpd_mcs;
  return shift_register;
}

void ShiftRegister_clear(ShiftRegister* shift_register)
{
  for (uint16_t i = 0; i < shift_register->size; i++)
    ShiftRegister_write(shift_register, false);
}

void ShiftRegister_write(ShiftRegister* shift_register, bool value)
{
  Component_set_write(&shift_register->data);
  Component_write(&shift_register->clk, false);
  Component_write(&shift_register->data, value);
  Component_write(&shift_register->clk, true);
  for (uint8_t i = 0; i < shift_register->tpd_mcs; i++)
    _delay_us(1); // because _delay_us expects a compile time constant
}

void ShiftRegister_write_n(ShiftRegister* shift_register, ...)
{
  va_list args;
  va_start(args, shift_register);
  for (uint16_t i = 0; i < shift_register->size; i++)
  {
    bool value = va_arg(args, int); // bool gets promoted to int through va_args
    ShiftRegister_write(shift_register, value);
  }
  va_end(args);
}