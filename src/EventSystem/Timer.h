#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "../Misc/Actor.h"

typedef void(*TimerCallback)();

typedef struct Timer {
  uint64_t interval_ms;
  uint64_t last_time_ms;
  TimerCallback callback;

  void(*start)();
  void(*stop)();
  DECLARE_ACTOR_MEM(Timer);
} Timer;

DECLARE_ACTOR(Timer);

Timer Timer_create(uint64_t interval_ms, TimerCallback callback);
void Timer_start(Timer* timer);
void Timer_stop(Timer* timer);
void Timer_update(void* obj);

#endif // TIMER_H