#ifndef SYSTEM_H
#define SYSTEM_H

#include "Models/ShiftRegister.h"
#include "Models/USART.h"

typedef struct
{
  ShiftRegister led_strip;    //< the LED strip (ShiftRegister)
  USART logger;               //< the logger (USART)
} System;

/// @brief creates a system with a shift register for the led strip and a usart as the logger
System System_create();

#endif // SYSTEM_H