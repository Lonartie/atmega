#ifndef HP_TIMER_H
#define HP_TIMER_H

#include <stdint.h>

typedef void(*HPTimerCallback)();

typedef struct {
  uint64_t interval_us;
  uint64_t last_time_us;
  HPTimerCallback callback;
} HPTimer;

HPTimer HPTimer_create(uint64_t interval_us, HPTimerCallback callback);
void HPTimer_start(HPTimer* timer);
void HPTimer_update(void* obj);

#endif // HP_TIMER_H