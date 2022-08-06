#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "States.h"

/// @brief current distance to the wall
extern uint8_t us_current_sensor_distance;

/// @brief recognized track direction
extern TrackDirection track_direction;

/// @brief whether or not a wall was detected
extern bool wall_detected;

/// @brief when the last direction update was
extern uint32_t last_direction_update;

/// @brief whether or not the track direction may be updated
extern bool update_track_direction;

/// @brief current obstacle avoidance phase
extern uint8_t wall_phase;

/// @brief last obstacle avoidance phase
extern uint8_t last_wall_phase;

/// @brief when the last measurement was
extern uint32_t last_measure;

/// @brief when the last last measurement was
extern uint32_t last_last_measure;

/// @brief when the smooth steering started
extern uint32_t smooth_steer_start;

/// @brief whether or not the smooth steering is active
extern bool is_smooth_steering;

/// @brief current command received through USART
extern char* current_command;

/// @brief whether or not the system is in a safe state
extern bool safe_state;

/// @brief when the last message was sent
extern uint32_t last_message_sent;

/// @brief when the last led blink was
extern uint32_t last_led_blink;

/// @brief when the last led update was (for the lauflicht)
extern uint32_t last_led_update;

/// @brief when the system started driving
extern uint32_t start_driving_time;

/// @brief whether or not obstacle avoidance is enabled
extern bool avoid_obstacles_enabled;

/// @brief whether or not the system is returning home
extern bool return_home;

/// @brief current round
extern uint8_t rounds;

/// @brief current presentation state
extern PresentationState presentation_state;

/// @brief whether or not the system is in py mode
extern bool py_mode;

#endif  // GLOBALS_H