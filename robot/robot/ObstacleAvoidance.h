#ifndef OBSTACLEAVOIDANCE_H
#define OBSTACLEAVOIDANCE_H

#include "Models/System.h"

/// @brief handler for the ultra sound sensor event. Checks for threshold and
/// sets global states accordingly
/// @param atmega the system
void detect_wall(void* system);

/// @brief the logic to avoid obstacle
/// @note keeps track of the different phases of the obstacle avoidance
/// @param atmega the system to drive
/// @param seees_wall whether or not the robot sees a wall
/// @param any_sensor whether or not any sensor sees something
void avoid_obstacle_logic(System* atmega, bool sees_wall, bool any_sensor);

/// @brief phase 0: turn around until wall is invisible
/// @param atmega the system to drive
/// @param seees_wall whether or not the robot sees a wall
void obstacle_phase_0(System* atmega, bool sees_wall);

/// @brief phase 1: turn around until wall is invisible
/// @param atmega the system to drive
/// @param seees_wall whether or not the robot sees a wall
void obstacle_phase_1(System* atmega, bool sees_wall);

/// @brief phase 2: turn around until wall is visible again
/// @param atmega the system to drive
/// @param seees_wall whether or not the robot sees a wall
void obstacle_phase_2(System* atmega, bool sees_wall);

/// @brief phase 3: drive forward until wall is invisible
/// @param atmega the system to drive
/// @param seees_wall whether or not the robot sees a wall
void obstacle_phase_3(System* atmega, bool sees_wall);

/// @brief phase 4: turn around until wall is visible again
/// @param atmega the system to drive
/// @param seees_wall whether or not the robot sees a wall
void obstacle_phase_4(System* atmega, bool sees_wall);

/// @brief reset phase (when roboter sees line again)
void obstacle_phase_reset(System* atmega);

/// @returns whether or not the wall measurement was recently enough
bool measure_was_recently();

#endif  // OBSTACLEAVOIDANCE_H