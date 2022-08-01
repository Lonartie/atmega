#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

#include "States.h"

static const int16_t SPEED_DRIVE = 160;
static const int16_t SPEED_TURN_A = 220;
static const int16_t SPEED_TURN_B = 220;
static const int16_t SPEED_TURN_SLOW_A = 240;
static const int16_t SPEED_TURN_SLOW_B = 20;

static const uint32_t TIME_TO_TRY_SMOOTH_STEERING_MS = 120;
static const uint32_t TIME_TO_RECOGNIZE_START_BLOCK_MS = 100;

static const uint8_t US_SENSOR_DISTANCE_SMALL = 16;
static const uint8_t US_SENSOR_DISTANCE_LARGE = 23;
static uint8_t US_CURRENT_SENSOR_DISTANCE = 16;

static const uint32_t DIRECTION_UPDATE_DELAY_MS = 300;

static const uint32_t MEASURE_THRESHOLD_LEFT = 330;
static const uint32_t MEASURE_THRESHOLD_MID = 400;
static const uint32_t MEASURE_THRESHOLD_RIGHT = 330;

static const char* COMMANDS_STR =
    "?  -> show help\n"
    "S  -> start driving\n"
    "R  -> reset in 5 seconds\n"
    "X  -> safe state\n";
static const char* DRIVING_COMMANDS_STR =
    "P  -> pause/unpause driving\n"
    "C  -> return home\n";

static const char* IDLE_MESSAGE =
    "Not on the starting field. Place me there please... Send ? for help\n";
static const char* START_BLOCK_MESSAGE =
    "On the starting field. Waiting for your instructions... Send ? for help\n";
static const char* START_ROUND_ONE_MESSAGE =
    "Here I go again on my own, going down the only round I've ever known\n";
static const char* START_ROUND_TWO_MESSAGE =
    "YEAH, done round 1, going for round 2/3\n";
static const char* START_ROUND_THREE_MESSAGE =
    "YEAH YEAH YEAH, done round 2, going for round 3/3\n";
static const char* END_MESSAGE =
    "YEAH YEAH YEAH, I really did it my way... And waht's my purpose and the "
    "general sense of my further life now? Will reset myself in 5 seconds. "
    "Take care of you and me!\n";
static const char* ROUND_MESSAGE = "Round and round I go, currently round %d\n";
static const char* MANUAL_RESET_MESSAGE =
    "Will reset myself in 5 seconds. I will forget everything. Make sure to "
    "handle me well and take care of my messages when I am back functioning. "
    "Thanks!\n";
static const char* PAUSE_MESSAGE =
    "Pause ... zzzZZZzzzZZZzzz ... wake me up with P again\n";
static const char* SAFE_SATE_MESSAGE =
    "In safe state! Won't react to any instructions! Rescue me!\n";

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
static bool safe_state = false;
static const uint32_t one_seconds_ms = 1000;
static const uint32_t five_seconds_ms = 5000;

static uint32_t last_message_sent = 0;
static uint32_t last_led_blink = 0;
static uint32_t last_led_update = 0;

static bool avoid_obstacles_enabled = false;
static uint8_t rounds = 0;
static PresentationState presentation_state = IDLE;

#endif  // CONSTANTS_H