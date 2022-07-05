#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void PWM_init();
void PWM_set_duty_cycle(uint8_t pin, uint8_t value);

#endif // PWM_H