#ifndef SYSTEM_H
#define SYSTEM_H

#include "Models/ShiftRegister.h"
#include "Models/USART.h"
#include "Models/Motor.h"
#include "Models/ADCPin.h"

/// @brief the abstraction of the microcontroller and all its components
///        currently configured for the ATmega328P-car-robot
typedef struct System
{
  ShiftRegister led_strip;    // the LED strip (ShiftRegister)
  ADCPin lf_left;             // the "Linienfolger" left (ADCPin)
  ADCPin lf_middle;           // the "Linienfolger" middle (ADCPin)
  ADCPin lf_right;            // the "Linienfolger" right (ADCPin)
  Motor mt_left;              // the left motor (Motor) 
  Motor mt_right;             // the right motor (Motor)
  bool started;
} System;

/// @brief creates a system with a shift register for the led strip and a usart as the logger
System System_create();
void System_start(void* _this);
void System_stop(void* _this);
void System_drive(void* _this);

#endif // SYSTEM_H