#ifndef OBSTACLEAVOIDANCE_H
#define OBSTACLEAVOIDANCE_H

#include "Models/System.h"

void detect_wall(void* system);
void avoid_obstacle_logic(System* atmega, bool sees_wall, bool any_sensor);
void obstacle_phase_reset(System* atmega);
void obstacle_phase_0(System* atmega, bool sees_wall);
void obstacle_phase_1(System* atmega, bool sees_wall);
void obstacle_phase_2(System* atmega, bool sees_wall);
void obstacle_phase_3(System* atmega, bool sees_wall);
void obstacle_phase_4(System* atmega, bool sees_wall);
bool measure_was_recently();

#endif  // OBSTACLEAVOIDANCE_H