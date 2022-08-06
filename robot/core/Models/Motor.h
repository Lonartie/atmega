#ifndef MOTOR_H
#define MOTOR_H

#include "Misc/Utils.h"
#include "Pin.h"

/// @brief represents a motor with a forward and a backward driving pin
/// and a pwm pin
typedef struct Motor {
  Pin forward;
  Pin backward;
  uint8_t pwm_pin;
} Motor;

/// @brief creates a new motor with the given pins
/// @param pwm_pin the pin to use for PWM
/// @param forward the pin to use for driving forward
/// @param backward the pin to use for driving backward
/// @returns a new motor
Motor Motor_create(uint8_t pwm_pin, Pin forward, Pin backward);

/// @brief drives the motor forward with the given speed
/// @param _this the motor to drive
/// @param speed the speed to drive the motor at
void Motor_drive_forward(Motor* _this, uint8_t speed);

/// @brief drives the motor backward with the given speed
/// @param _this the motor to drive
/// @param speed the speed to drive the motor at
void Motor_drive_backward(Motor* _this, uint8_t speed);

/// @brief stops the motor
/// @param _this the motor to stop
void Motor_stop(Motor* _this);

#endif  // MOTOR_H