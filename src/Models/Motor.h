#ifndef MOTOR_H
#define MOTOR_H

#include "../Misc/utils.h"
#include "Pin.h"

typedef struct Motor
{
  Pin forward;
  Pin backward;
  uint8_t pwm_pin;

  void(*drive_forward)();
  void(*drive_backward)();
  void(*set_speed)(uint8_t speed);
  void(*stop)();
} Motor;

Motor Motor_create(uint8_t pwm_pin, Pin forward, Pin backward);
void Motor_drive_forward(Motor* _this);
void Motor_drive_backward(Motor* _this);
void Motor_set_speed(Motor* _this, uint8_t speed);
void Motor_stop(Motor* _this);


#endif // MOTOR_H