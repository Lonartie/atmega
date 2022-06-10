#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include "Component.h"

typedef struct {
  Component clk;    ///< clock component
  Component data;   ///< data component
  uint16_t size;    ///< size of the register
  uint16_t tpd_mcs; ///< time per data bit in microseconds
} ShiftRegister;

/// @returns a new shift register (stack)
ShiftRegister ShiftRegister_create(
  volatile uint8_t* clk_ddr, volatile uint8_t* clk_port, uint8_t clk_pin,
  volatile uint8_t* data_ddr, volatile uint8_t* data_port, uint8_t data_pin,
  uint16_t size, uint16_t tpd_mcs);

/// @brief clears the shift register
void ShiftRegister_clear(ShiftRegister* sr);

/// @brief writes a value to the shift register
void ShiftRegister_write(ShiftRegister* sr, bool value);

/// @brief writes a number of values to the shift register
void ShiftRegister_write_n(ShiftRegister* sr, ...);

#endif // SHIFT_REGISTER_H