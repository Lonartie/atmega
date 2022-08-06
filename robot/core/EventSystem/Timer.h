#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#include "../Misc/String.h"

/// @brief the callback type which is called by the timer
typedef void (*TimerCallback)();

/// @brief a timer that sends events in millisecond intervals
typedef struct Timer {
  uint32_t interval_ms;
  uint32_t last_time_ms;
  String event;
} Timer;

/// @brief creates a new timer with the specified event and interval
/// @param interval_ms the interval in milliseconds
/// @param event the event that will be sent when the timer is done
/// @returns a new timer which has not started yet
Timer Timer_create(uint32_t interval_ms, String event);

/// @brief starts the given timer
/// @param timer the timer to start
void Timer_start(Timer* timer);

/// @brief stops the given timer
/// @param timer the timer to stop
void Timer_stop(Timer* timer);

/// @brief updates the given timer
/// @param timer the timer to update
void Timer_update(void* timer);

#endif  // TIMER_H