#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "States.h"

extern uint8_t us_current_sensor_distance;
extern TrackDirection track_direction;
extern bool wall_detected;
extern uint32_t last_direction_update;
extern bool update_track_direction;
extern uint8_t wall_phase;
extern uint8_t last_wall_phase;
extern uint32_t last_measure;
extern uint32_t last_last_measure;
extern uint32_t smooth_steer_start;
extern bool is_smooth_steering;
extern char* current_command;
extern bool safe_state;
extern uint32_t last_message_sent;
extern uint32_t last_led_blink;
extern uint32_t last_led_update;
extern uint32_t start_driving_time;
extern bool avoid_obstacles_enabled;
extern bool return_home;
extern uint8_t rounds;
extern PresentationState presentation_state;

#endif  // GLOBALS_H