#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include "Pin.h"
#include "../Misc/utils.h"

typedef struct ShiftRegister {
  Pin clk;          ///< clock component
  Pin data;         ///< data component
  uint16_t size;    ///< size of the register
  uint16_t tpd_mcs; ///< time per data bit in microseconds

  /// @brief clears the shift register by setting all flip flops to 0
  void(*clear)();

  /// @brief writes a value to the shift register
  void(*write)(bool);

  /// @brief writes a number of values to the shift register
  /// @note first value is written first so will end up on last pin after shifting
  void(*write_n)(uint32_t, ...);
} ShiftRegister;

/// @returns a new shift register (stack)
ShiftRegister ShiftRegister_create(
  volatile uint8_t* clk_ddr, volatile uint8_t* clk_port, uint8_t clk_ddr_pin, uint8_t clk_port_pin,
  volatile uint8_t* data_ddr, volatile uint8_t* data_port, uint8_t data_ddr_pin, uint8_t data_port_pin,
  uint16_t size, uint16_t tpd_mcs);

/// @brief clears the shift register
void ShiftRegister_clear(ShiftRegister* sr);

/// @brief writes a value to the shift register
void ShiftRegister_write(ShiftRegister* sr, bool value);

/// @brief writes a number of values to the shift register
/// @note first value is written first so will end up on last pin after shifting
void ShiftRegister_write_n(ShiftRegister* sr, uint32_t n, ...);

#endif // SHIFT_REGISTER_H