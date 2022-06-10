#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

typedef void(*TimerCallback)();

typedef struct {
  uint64_t interval_ms;
  uint64_t last_time_ms;
  TimerCallback callback;
} Timer;

Timer Timer_create(uint64_t interval_ms, TimerCallback callback);
void Timer_update(void* obj);

#endif // TIMER_H