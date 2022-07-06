#ifndef PHASED_TIMER_H
#define PHASED_TIMER_H

#include "../Misc/String.h"
#include "../Misc/Vector.h"
#include "Misc/Utils.h"

/// @brief a phased timer that can send events with different intervals (up to
/// 8)
typedef struct PhasedTimer {
  Vector_long_8 phases;
  String event;
  uint64_t last_time_ms;
  uint8_t phase;
} PhasedTimer;

/// @brief creates a new phased timer with the specified event and phases
/// @param event the event that will be sent when the timer is done
/// @param n the number of phases
/// @param ... the phases (int)
PhasedTimer PhasedTimer_create(String event, uint8_t n, ...);

/// @brief starts the given timer
void PhasedTimer_start(PhasedTimer* timer);

/// @brief stops the given timer
void PhasedTimer_stop(PhasedTimer* timer);

/// @brief updates the given timer
void PhasedTimer_update(void* obj);

#endif  // PHASED_TIMER_H