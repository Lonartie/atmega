#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "States.h"

#define SPEED_DRIVE 160
#define SPEED_DRIVE_SLOW 120
#define SPEED_TURN_A 220
#define SPEED_TURN_B 220
#define SPEED_TURN_SLOW_A 240
#define SPEED_TURN_SLOW_B 20

#define TIME_TO_TRY_SMOOTH_STEERING_MS 120
#define TIME_TO_RECOGNIZE_START_BLOCK_MS 100

#define US_SENSOR_DISTANCE_SMALL 16
#define US_SENSOR_DISTANCE_LARGE 23

#define DIRECTION_UPDATE_DELAY_MS 300

#define MEASURE_THRESHOLD_LEFT 330
#define MEASURE_THRESHOLD_MID 400
#define MEASURE_THRESHOLD_RIGHT 330

#define HELP_COMMAND_STR "?  -> show help\n"
#define START_COMMAND_STR "S  -> start driving\n"
#define RESET_COMMAND_STR "R  -> reset in 5 seconds\n"
#define SAFE_STATE_COMMAND_STR "X  -> safe state\n"
#define OBSTACLE_COMMAND_STR "A  -> toggle obstacle avoidance\n"
#define PAUSE_COMMAND_STR "P  -> pause/unpause driving\n"
#define RETURN_COMMAND_STR "C  -> return home\n"
#define PY_COMMAND_STR "PY -> start py command mode\n"

#define IDLE_MESSAGE \
  "Not on the starting field. Place me there please... Send ? for help\n"

#define START_BLOCK_MESSAGE \
  "On the starting field. Waiting for your instructions... Send ? for help\n"

#define START_ROUND_ONE_MESSAGE \
  "Here I go again on my own, going down the only round I've ever known\n"

#define START_ROUND_TWO_MESSAGE "YEAH, done round 1, going for round 2/3\n"

#define START_ROUND_THREE_MESSAGE \
  "YEAH YEAH YEAH, done round 2, going for round 3/3\n"

#define END_MESSAGE                                                          \
  "YEAH YEAH YEAH, I really did it my way... And waht's my purpose and the " \
  "general sense of my further life now? Will reset myself in 5 seconds. "   \
  "Take care of you and me!\n"

#define ROUND_MESSAGE "Round and round I go, currently round #%d\n"

#define MANUAL_RESET_MESSAGE                                                 \
  "Will reset myself in 5 seconds. I will forget everything. Make sure to "  \
  "handle me well and take care of my messages when I am back functioning. " \
  "Thanks!\n"

#define PAUSE_MESSAGE "Pause ... zzzZZZzzzZZZzzz ... wake me up with P again\n"

#define SAFE_SATE_MESSAGE \
  "In safe state! Won't react to any instructions! Rescue me!\n"

#define RETURN_HOME_MESSAGE \
  "Aborting this journey, hurrying home, will reset me there\n"

#define RETURN_HOME_END_MESSAGE                                               \
  "I just arrived at home. Resetting NOW! Take care of my messages when I'm " \
  "back...\n"

#define ONE_SECONDS_MS 1000
#define FIVE_SECONDS_MS 5000
#define WALL_MEASUREMENT_RECENTLY_THRESHOLD_MS 50

#endif  // CONSTANTS_H