#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

#include "States.h"

const int16_t SPEED_DRIVE = 160;
const int16_t SPEED_TURN_A = 220;
const int16_t SPEED_TURN_B = 220;
const int16_t SPEED_TURN_SLOW_A = 240;
const int16_t SPEED_TURN_SLOW_B = 20;

const uint16_t TIME_TO_TRY_SMOOTH_STEERING_MS = 120;
const uint16_t TIME_TO_RECOGNIZE_START_BLOCK_MS = 100;

const uint8_t US_SENSOR_DISTANCE_SMALL = 16;
const uint8_t US_SENSOR_DISTANCE_LARGE = 23;
static uint8_t US_CURRENT_SENSOR_DISTANCE = 16;

const uint16_t DIRECTION_UPDATE_DELAY_MS = 300;

const uint16_t MEASURE_THRESHOLD_LEFT = 330;
const uint16_t MEASURE_THRESHOLD_MID = 400;
const uint16_t MEASURE_THRESHOLD_RIGHT = 330;

const char* COMMANDS_STR =
    "?  -> show help\n"
    "S  -> start driving\n"
    "R  -> reset in 5 seconds\n"
    "X  -> safe state\n";
const char* DRIVING_COMMANDS_STR =
    "P  -> pause/unpause driving\n"
    "C  -> return home\n";

const char* IDLE_MESSAGE =
    "Not on the starting field. Place me there please... Send ? for help\n";
const char* START_BLOCK_MESSAGE =
    "On the starting field. Waiting for your instructions... Send ? for help\n";
const char* START_ROUND_ONE_MESSAGE =
    "Here I go again on my own, going down the only round I've ever known\n";
const char* START_ROUND_TWO_MESSAGE =
    "YEAH, done round 1, going for round 2/3\n";
const char* START_ROUND_THREE_MESSAGE =
    "YEAH YEAH YEAH, done round 2, going for round 3/3\n";
const char* END_MESSAGE =
    "YEAH YEAH YEAH, I really did it my way... And waht's my purpose and the "
    "general sense of my further life now? Will reset myself in 5 seconds. "
    "Take care of you and me!\n";
const char* ROUND_MESSAGE = "Round and round I go, currently round %d\n";
const char* MANUAL_RESET_MESSAGE =
    "WIll reset myself in 5 seconds. I will forget everything. Make sure to "
    "handle me well and take care of my messages when I am back functioning. "
    "Thanks!\n";
const char* PAUSE_MESSAGE =
    "Pause ... zzzZZZzzzZZZzzz ... wake me up with P again\n";
const char* SAFE_SATE_MESSAGE =
    "In safe state! Won't react to any instructions! Rescue me!\n";

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
static uint16_t last_direction_update = 0;
static bool update_track_direction = true;
static uint8_t wall_phase = 0;
static uint8_t last_wall_phase = UINT8_MAX;
static uint16_t last_measure = 0;
static uint16_t last_last_measure = 0;
static uint16_t smooth_steer_start = 0;
static bool is_smooth_steering = false;
static char* current_command = NULL;
static bool safe_state = false;

static bool avoid_obstacles_enabled = false;
static uint8_t rounds = 0;
static PresentationState presentation_state = IDLE;

#endif  // CONSTANTS_H