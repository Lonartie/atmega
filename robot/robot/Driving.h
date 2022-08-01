#ifndef DRIVING_H
#define DRIVING_H

#include "Models/System.h"

#define ROUNDS_COUNTER_START_DELAY_MS 1500

void turn_left(System* atmega);
void turn_right(System* atmega);
void turn_smooth_left(System* atmega);
void turn_smooth_right(System* atmega);
void drive_forward(System* atmega);
void stop_driving(System* atmega);

void drive(System* atmega, bool left, bool mid, bool right, bool sees_wall);

#endif  // DRIVING_H