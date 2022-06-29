#ifndef SERVO_H
#define SERVO_H

#include "Pin.h"
#include <stdint.h>

typedef struct Servo {
  Pin pin;
  volatile uint8_t* reg;
  uint8_t delay_ms;
} Servo;

Servo Servo_create(Pin pin, volatile uint8_t* reg, uint8_t delay_ms);
void Servo_set_angle(Servo* _this, char angle);

#endif // SERVO_H