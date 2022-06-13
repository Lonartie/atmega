#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "../Misc/String.h"
#include "../Misc/Actor.h"

typedef void(*TimerCallback)();

typedef struct Timer {
  uint64_t interval_ms;
  uint64_t last_time_ms;
  String event_type;

  void(*start)();
  void(*stop)();
} Timer;

Timer Timer_create(uint64_t interval_ms, String event_type);
void Timer_start(Timer* timer);
void Timer_stop(Timer* timer);
void Timer_update(void* obj);

#endif // TIMER_H