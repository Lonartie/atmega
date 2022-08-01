#ifndef DRIVING_H
#define DRIVING_H

#include "Models/System.h"

void turn_left(System* atmega);
void turn_right(System* atmega);
void turn_smooth_left(System* atmega);
void turn_smooth_right(System* atmega);
void drive_forward(System* atmega);
void stop_driving(System* atmega);

void drive(System* atmega, bool left, bool mid, bool right, bool sees_wall);
void count_rounds(System* atmega, bool left, bool mid, bool right);

#endif  // DRIVING_H