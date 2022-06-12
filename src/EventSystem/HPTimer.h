#ifndef HPTIMER_H
#define HPTIMER_H

#include <stdint.h>
#include "String.h"
#include "../Misc/Actor.h"

typedef void(*TimerCallback)();

typedef struct HPTimer {
  uint64_t interval_us;
  uint64_t last_time_us;
  String event_type;

  void(*start)();
  void(*stop)();
} HPTimer;

HPTimer HPTimer_create(uint64_t interval_us, String event_type);
void HPTimer_start(HPTimer* timer);
void HPTimer_stop(HPTimer* timer);
void HPTimer_update(void* obj);

#endif // TIMER_H