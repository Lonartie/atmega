#include "Globals.h"

uint8_t us_current_sensor_distance = 16;
TrackDirection track_direction = TD_UNKNOWN;
bool wall_detected = false;
uint32_t last_direction_update = 0;
bool update_track_direction = true;
uint8_t wall_phase = 0;
uint8_t last_wall_phase = UINT8_MAX;
uint32_t last_measure = 0;
uint32_t last_last_measure = 0;
uint32_t smooth_steer_start = 0;
bool is_smooth_steering = false;
char* current_command = NULL;
bool safe_state = false;

uint64_t last_message_sent_us = 0;
uint32_t last_message_sent = 0;
uint32_t last_led_blink = 0;
uint32_t last_led_update = 0;

uint32_t start_driving_time = 0;

bool avoid_obstacles_enabled = false;
bool return_home = false;
uint8_t rounds = 0;
PresentationState presentation_state = PS_IDLE;
bool py_mode = false;