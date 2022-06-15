#ifndef HPTIMER_H
#define HPTIMER_H

#include <stdint.h>
#include "../Misc/String.h"
#include "../Misc/Actor.h"

/// @brief a high precision timer that sends events in microsecond intervals
typedef struct HPTimer {
  uint64_t interval_us;
  uint64_t last_time_us;
  String event;

  /// @brief starts the timer
  void(*start)();

  /// @brief stops the timer
  void(*stop)();
} HPTimer;

/// @brief creates a new high precision timer with the specified event and interval
HPTimer HPTimer_create(uint64_t interval_us, String event);

/// @brief starts the given timer
void HPTimer_start(HPTimer* timer);

/// @brief stops the given timer
void HPTimer_stop(HPTimer* timer);

/// @brief updates the given timer
void HPTimer_update(void* obj);

#endif // TIMER_H