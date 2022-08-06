#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include "Misc/Utils.h"
#include "Pin.h"

/// @brief represents a shift register with a data pin, clock pin
/// @note this struct expects to know the size of the shift register
typedef struct ShiftRegister {
  Pin clk;          // clock component
  Pin data;         // data component
  uint16_t size;    // size of the register
  uint16_t tpd_us;  // time per data bit in microseconds
} ShiftRegister;

/// @brief creates a new shift register with the given pins
/// @param clk_ddr the data direction register of the clock pin
/// @param clk_port the port register of the clock pin
/// @param clk_ddr_pin the ddr pin number of the clock pin
/// @param clk_port_pin the port pin number of the clock pin
/// @param data_ddr the data direction register of the data pin
/// @param data_port the port register of the data pin
/// @param data_ddr_pin the ddr pin number of the data pin
/// @param data_port_pin the port pin number of the data pin
/// @param size the size of the shift register
/// @param tpd_us the delay time to wait between clock pulses in
ShiftRegister ShiftRegister_create(volatile uint8_t* clk_ddr,
                                   volatile uint8_t* clk_port,
                                   uint8_t clk_ddr_pin, uint8_t clk_port_pin,
                                   volatile uint8_t* data_ddr,
                                   volatile uint8_t* data_port,
                                   uint8_t data_ddr_pin, uint8_t data_port_pin,
                                   uint16_t size, uint16_t tpd_us);

/// @brief clears the shift register
/// @param sr the shift register to clear
void ShiftRegister_clear(ShiftRegister* sr);

/// @brief writes a value to the shift register
/// @param sr the shift register to write to
/// @param value the value to write to the shift register
void ShiftRegister_write(ShiftRegister* sr, bool value);

/// @brief writes a number of values to the shift register
/// @note first value is written first so will end up on last pin after shifting
/// @param sr the shift register to write to
/// @param n the number of values to write to the shift register
/// @param ... the values to write to the shift register
void ShiftRegister_write_n(ShiftRegister* sr, uint32_t n, ...);

#endif  // SHIFT_REGISTER_H