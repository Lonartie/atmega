#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "States.h"

/// @brief the speed the pwm is set to when driving normally
#define SPEED_DRIVE 160

/// @brief the speed the pwm is set to when driving slowly
#define SPEED_DRIVE_SLOW 120

/// @brief the speed the pwm is set to for the faster side when turning
#define SPEED_TURN_A 220

/// @brief the speed the pwm is set to for the slower side when turning
#define SPEED_TURN_B 220

/// @brief the speed the pwm is set to for the faster side when turning slowly
#define SPEED_TURN_SLOW_A 240

/// @brief the speed the pwm is set to for the slower side when turning slowly
#define SPEED_TURN_SLOW_B 20

/// @brief the time in ms to try to steer smoothly before steering hard
#define TIME_TO_TRY_SMOOTH_STEERING_MS 80

/// @brief the time in ms it needs to recognize the start block
#define TIME_TO_RECOGNIZE_START_BLOCK_MS 100

/// @brief the distance in cm for recognizing the wall in the first place
#define US_SENSOR_DISTANCE_SMALL 16

/// @brief the distance in cm to keep from the wall while avoiding the obstacle
#define US_SENSOR_DISTANCE_LARGE 23

/// @brief the delay in ms it takes before recognizing a hard corner
#define DIRECTION_UPDATE_DELAY_MS 300

/// @brief the analog measurement threshold for the left infrared sensor
#define MEASURE_THRESHOLD_LEFT 330

/// @brief the analog measurement threshold for the right infrared sensor
#define MEASURE_THRESHOLD_MID 400

/// @brief the analog measurement threshold for the right infrared sensor
#define MEASURE_THRESHOLD_RIGHT 330

/// @brief the command string for showing the help menu
#define HELP_COMMAND_STR "?  -> show help\n"

/// @brief the command string to start driving
#define START_COMMAND_STR "S  -> start driving\n"

/// @brief the command string to reset the system in 5 seconds
#define RESET_COMMAND_STR "R  -> reset in 5 seconds\n"

/// @brief the command string for safe state
#define SAFE_STATE_COMMAND_STR "X  -> safe state\n"

/// @brief the command string to enable / disable obstacle avoidance
#define OBSTACLE_COMMAND_STR "A  -> toggle obstacle avoidance\n"

/// @brief the command string to enable / disable pause
#define PAUSE_COMMAND_STR "P  -> pause/unpause driving\n"

/// @brief the command string to drive home and reset there
#define RETURN_COMMAND_STR "C  -> return home\n"

/// @brief the command string to start PY mode
#define PY_COMMAND_STR "PY -> start py command mode\n"

/// @brief idle message when not started yet and not on start block
#define IDLE_MESSAGE \
  "Not on the starting field. Place me there please... Send ? for help\n"

/// @brief message when not started yet and standing to the start block
#define START_BLOCK_MESSAGE \
  "On the starting field. Waiting for your instructions... Send ? for help\n"

/// @brief message when starting round #1
#define START_ROUND_ONE_MESSAGE \
  "Here I go again on my own, going down the only round I've ever known\n"

/// @brief message when starting round #2
#define START_ROUND_TWO_MESSAGE "YEAH, done round 1, going for round 2/3\n"

/// @brief message when starting round #3
#define START_ROUND_THREE_MESSAGE \
  "YEAH YEAH YEAH, done round 2, going for round 3/3\n"

/// @brief message when completing round #3
#define END_MESSAGE                                                          \
  "YEAH YEAH YEAH, I really did it my way... And waht's my purpose and the " \
  "general sense of my further life now? Will reset myself in 5 seconds. "   \
  "Take care of you and me!\n"

/// @brief message to show constantly when driving the rounds
#define ROUND_MESSAGE "Round and round I go, currently round #%d\n"

/// @brief message for manual reset
#define MANUAL_RESET_MESSAGE                                                 \
  "Will reset myself in 5 seconds. I will forget everything. Make sure to "  \
  "handle me well and take care of my messages when I am back functioning. " \
  "Thanks!\n"

/// @brief message when pause is enabled
#define PAUSE_MESSAGE "Pause ... zzzZZZzzzZZZzzz ... wake me up with P again\n"

/// @brief message when safe state is enabled
#define SAFE_SATE_MESSAGE \
  "In safe state! Won't react to any instructions! Rescue me!\n"

/// @brief message when returning home
#define RETURN_HOME_MESSAGE \
  "Aborting this journey, hurrying home, will reset me there\n"

/// @brief message when gone home
#define RETURN_HOME_END_MESSAGE                                               \
  "I just arrived at home. Resetting NOW! Take care of my messages when I'm " \
  "back...\n"

/// @brief constant for one second in ms
#define ONE_SECONDS_MS 1000

/// @brief constant for five seconds in ms
#define FIVE_SECONDS_MS 5000

/// @brief threshold for when to not consider the measurement recently anymore
#define WALL_MEASUREMENT_RECENTLY_THRESHOLD_MS 50

#endif  // CONSTANTS_H