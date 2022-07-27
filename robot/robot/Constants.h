#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

#include "States.h"

const int8_t SPEED_DRIVE_SLOW = 0;
const int16_t SPEED_DRIVE = 220;
const int16_t SPEED_TURN_A = 220;
const int16_t SPEED_TURN_B = 220;
const int16_t SPEED_TURN_SLOW_A = 240;
const int16_t SPEED_TURN_SLOW_B = 20;

const uint32_t TIME_TO_TRY_SMOOTH_STEERING_US = 150000;

const uint8_t US_SENSOR_DISTANCE_SMALL = 16;
const uint8_t US_SENSOR_DISTANCE_LARGE = 23;
static uint8_t US_CURRENT_SENSOR_DISTANCE = 16;

const uint32_t DIRECTION_UPDATE_DELAY_US = 300000;

const uint16_t MEASURE_THRESHOLD_LEFT = 330;
const uint16_t MEASURE_THRESHOLD_MID = 400;
const uint16_t MEASURE_THRESHOLD_RIGHT = 330;

const char* COMMANDS_STR =
    "?  -> show help\n"
    "S  -> start driving\n";
const char* IDLE_MESSAGE =
    "Not on the starting field. Place me there please... Send ? for help\n";
const char* START_BLOCK_MESSAGE =
    "On the starting field. Waiting for your instructions... Send ? for help\n";

const char* TURN_LEFT_MESSAGE = "tl\n";
const char* TURN_RIGHT_MESSAGE = "tr\n";
const char* DRIVE_FORWARD_MESSAGE = "df\n";
const char* STOP_MESSAGE = "s\n";
const char* TIMER_MESSAGE = "%ums\n";
const char* SENSORS_MESSAGE = "%d%d%d\n";
const char* SENSORS_DEBUG_MESSAGE = "%d %d %d\n";
const char* US_SENSOR_MESSAGE = "dist: %d\n";

static TrackDirection track_direction = TRACK_UNKNOWN;
static bool wall_detected = false;
static uint32_t last_direction_update = 0;
static bool update_track_direction = true;
static uint8_t wall_phase = 0;
static uint8_t last_wall_phase = UINT8_MAX;
static uint32_t last_measure = 0;
static uint32_t last_last_measure = 0;
static uint32_t smooth_steer_start = 0;
static bool is_smooth_steering = false;
static char* current_command = NULL;

static uint8_t rounds = 0;
static PresentationState presentation_state = IDLE;

#endif  // CONSTANTS_H