#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

#include "Pin.h"

/// @brief represents a servo motor
typedef struct Servo {
  Pin pin;                // pin to which the servo is connected
  volatile uint8_t* reg;  // register where the servo stores its position
  uint8_t delay_ms;  // delay to wait for the servo to move to the next position
} Servo;

/// @brief creates a new servo motor with the given pin
/// @param pin the pin to which the servo is connected
/// @param reg the register where the servo stores its position
/// @param delay_ms the delay to wait for the servo to move to the next position
/// @returns a new servo motor
Servo Servo_create(Pin pin, volatile uint8_t* reg, uint8_t delay_ms);

/// @brief sets the servo to the given position
/// @param _this the servo to set the position of
/// @param angle the angle to set the servo to (-90 to 90)
void Servo_set_angle(Servo* _this, char angle);

#endif  // SERVO_H