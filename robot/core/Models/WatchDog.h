#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "Misc/Utils.h"

/// @brief the time intervals supported by the watchdog
typedef enum WatchDogTime {
  NOW,     // immediate reset (actually 15ms but who cares)
  MS_500,  // 500 milliseconds
  SEC_1,   // 1 second
  SEC_2,   // 2 seconds
  SEC_4,   // 4 seconds
  SEC_8    // 8 seconds
} WatchDogTime;

/// @brief starts the watchdog with the given time interval
/// @note after system reset make sure to call stop_watchdog() !
/// @param time the time interval to use
void watchdog_init(WatchDogTime time);

/// @brief stops the watchdog timer
void stop_watchdog();

/// @brief resets the watchdog timer so the interval is restarting
void reset_watchdog();

#endif  // WATCHDOG_H