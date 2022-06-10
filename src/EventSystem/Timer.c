#include "Timer.h"
#include "EventQueue.h"
#include "HardwareTimer.h"

Timer Timer_create(uint64_t interval_ms, TimerCallback callback)
{
  Timer timer;
  timer.interval_ms = interval_ms;
  timer.callback = callback;
  timer.last_time_ms = millis();
  return timer;
}

void Timer_start(Timer* timer)
{
  EventQueue_register_updater(Updater_create(timer, Timer_update));
}

void Timer_update(void* obj)
{
  Timer* timer = (Timer*) obj;
  uint64_t current_time_ms = millis();
  if (timer->last_time_ms + timer->interval_ms <= current_time_ms) {
    timer->last_time_ms = current_time_ms;
    timer->callback();
  }
}