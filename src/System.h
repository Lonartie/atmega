#ifndef SYSTEM_H
#define SYSTEM_H

#include "Models/ShiftRegister.h"
#include "Models/USART.h"

typedef struct
{
  ShiftRegister led_strip;
  USART logger;
} System;

System System_create();

#endif // SYSTEM_H