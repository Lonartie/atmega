#ifndef MOTOR_H
#define MOTOR_H

#include "Misc/Utils.h"
#include "Pin.h"

typedef struct Motor
{
  Pin forward;
  Pin backward;
  uint8_t pwm_pin;
} Motor;

Motor Motor_create(uint8_t pwm_pin, Pin forward, Pin backward);
void Motor_drive_forward(Motor* _this, uint8_t speed);
void Motor_drive_backward(Motor* _this, uint8_t speed);
void Motor_stop(Motor* _this);


#endif // MOTOR_H