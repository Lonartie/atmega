#ifndef PWM_H
#define PWM_H

#include <stdint.h>

/// @brief initializes the pwm timer
void PWM_init();

/// @brief sets the pwm duty cycle for the given pin
/// @param pin the pin to set the duty cycle for
/// @param value the duty cycle to set the pin to
void PWM_set_duty_cycle(uint8_t pin, uint8_t value);

#endif  // PWM_H