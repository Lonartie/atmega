#ifndef PHASED_TIMER_H
#define PHASED_TIMER_H

#include "../Misc/utils.h"
#include "../Misc/String.h"
#include "../Misc/Vector.h"

typedef struct PhasedTimer
{
  Vector_long_8 phases;
  String event;
  uint64_t last_time_ms;
  uint8_t phase;

  void(*start)();
  void(*stop)();
} PhasedTimer;

PhasedTimer PhasedTimer_create(String event, uint8_t n, ...);
void PhasedTimer_start(PhasedTimer* timer);
void PhasedTimer_stop(PhasedTimer* timer);
void PhasedTimer_update(void* obj);

#endif // PHASED_TIMER_H