#ifndef DRIVING_H
#define DRIVING_H

#include "Models/System.h"

/// @brief turns smooth left for DIRECTION_UPDATE_DELAY_MS and then fully left
/// @param atmega the system to drive
void turn_left(System* atmega);

/// @brief turns smooth right for DIRECTION_UPDATE_DELAY_MS and then fully right
/// @param atmega the system to drive
void turn_right(System* atmega);

/// @brief turns smooth left
/// @param atmega the system to drive
void turn_smooth_left(System* atmega);

/// @brief turns smooth right
/// @param atmega the system to drive
void turn_smooth_right(System* atmega);

/// @brief drives forward
/// @param atmega the system to drive
/// @param left whether or not the left line sensor sees something
/// @param mid whether or not the mid line sensor sees something
/// @param right whether or not the right line sensor sees something
void drive_forward(System* atmega, bool left, bool mid, bool right);

/// @brief drives backward
/// @param atmega the system to drive
void drive_backward(System* atmega);

/// @brief stops the motors
/// @param atmega the system to drive
void stop_driving(System* atmega);

/// @brief general driving logic to stay on the track, avoid obstacles and send
/// messages
/// @param atmega the system to drive
/// @param left whether or not the left line sensor sees something
/// @param mid whether or not the mid line sensor sees something
/// @param right whether or not the right line sensor sees something
/// @param sees_wall whether or not the robot sees a wall
void drive(System* atmega, bool left, bool mid, bool right, bool sees_wall);

/// @brief counts the number of rounds and sends the corresponding message or
/// resets
/// @param atmega the system to drive
/// @param left whether or not the left line sensor sees something
/// @param mid whether or not the mid line sensor sees something
/// @param right whether or not the right line sensor sees something
void count_rounds(System* atmega, bool left, bool mid, bool right);

#endif  // DRIVING_H