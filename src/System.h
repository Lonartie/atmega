#ifndef SYSTEM_H
#define SYSTEM_H

#include "Models/ShiftRegister.h"
#include "Models/USART.h"
#include "Models/Motor.h"

/// @brief the abstraction of the microcontroller and all its components
///        currently configured for the ATmega328P-car-robot
typedef struct System
{
  ShiftRegister led_strip;    // the LED strip (ShiftRegister)
  USART logger;               // the logger (USART)
  Pin lf_left;                // the "Linienfolger" left (Pin)
  Pin lf_middle;              // the "Linienfolger" middle (Pin)
  Pin lf_right;               // the "Linienfolger" right (Pin)
  Motor mt_left;              // the left motor (Motor) 
  Motor mt_right;             // the right motor (Motor)
} System;

/// @brief creates a system with a shift register for the led strip and a usart as the logger
System System_create();

#endif // SYSTEM_H