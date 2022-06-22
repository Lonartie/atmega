#include "Motor.h"
#include "PWM.h"
#include <util/delay.h>

Motor Motor_create(uint8_t pwm_pin, Pin forward, Pin backward)
{
  Motor motor;
  motor.forward = forward;
  motor.backward = backward;
  motor.pwm_pin = pwm_pin;

  Pin_set_write(&motor.forward);
  Pin_set_write(&motor.backward);
  
  return motor;
}

void Motor_drive_forward(Motor* _this, uint8_t speed)
{
  // Pin_write(&_this->forward, 0);
  Pin_write(&_this->backward, 0); 
  // _delay_ms(1);
  Pin_write(&_this->forward, 1);
  PWM_set_duty_cycle(_this->pwm_pin, speed);
}

void Motor_drive_backward(Motor* _this, uint8_t speed)
{
  Pin_write(&_this->forward, 0);
  // Pin_write(&_this->backward, 0);
  //  _delay_ms(1);
  Pin_write(&_this->backward, 1);
  PWM_set_duty_cycle(_this->pwm_pin, speed);
}

void Motor_stop(Motor* _this)
{
  // PWM_set_duty_cycle(_this->pwm_pin, 0);
  Pin_write(&_this->forward, 0);
  Pin_write(&_this->backward, 0);
}