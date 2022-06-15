#include "Motor.h"
#include "PWM.h"
#include "../Misc/Actor.h"
#include <util/delay.h>

DEFINE_ACTOR_FORWARDER(void, Motor, drive_forward);
DEFINE_ACTOR_FORWARDER(void, Motor, drive_backward);
DEFINE_ACTOR_FORWARDER_N(void, Motor, set_speed, (uint8_t speed), (speed));
DEFINE_ACTOR_FORWARDER(void, Motor, stop);

Motor Motor_create(uint8_t pwm_pin, Pin forward, Pin backward)
{
  Motor motor;
  motor.forward = forward;
  motor.backward = backward;
  motor.pwm_pin = pwm_pin;

  SET_ACTOR_FORWARDER(motor, Motor, drive_forward);
  SET_ACTOR_FORWARDER(motor, Motor, drive_backward);
  SET_ACTOR_FORWARDER(motor, Motor, set_speed);
  SET_ACTOR_FORWARDER(motor, Motor, stop);

  return motor;
}

void Motor_drive_forward(Motor* _this)
{
  Pin_write(&_this->backward, 0);
  _delay_ms(1);
  Pin_write(&_this->forward, 1);
}

void Motor_drive_backward(Motor* _this)
{
  Pin_write(&_this->forward, 0);
  _delay_ms(1);
  Pin_write(&_this->backward, 1);
}

void Motor_set_speed(Motor* _this, uint8_t speed)
{
  PWM_set_duty_cycle(_this->pwm_pin, speed);
}

void Motor_stop(Motor* _this)
{
  PWM_set_duty_cycle(_this->pwm_pin, 0);
  Pin_write(&_this->forward, 0);
  Pin_write(&_this->backward, 0);
}