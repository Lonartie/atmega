#ifndef HP_TIMER_H
#define HP_TIMER_H

#include <stdint.h>
#include "../Misc/Actor.h"

typedef void(*HPTimerCallback)();

typedef struct HPTimer {
  uint64_t interval_us;
  uint64_t last_time_us;
  HPTimerCallback callback;

  void(*start)();
  void(*stop)();
} HPTimer;

HPTimer HPTimer_create(uint64_t interval_us, HPTimerCallback callback);
void HPTimer_start(HPTimer* timer);
void HPTimer_stop(HPTimer* timer);
void HPTimer_update(void* obj);

#endif // HP_TIMER_H