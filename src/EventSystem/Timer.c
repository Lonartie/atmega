#include "Timer.h"
#include "EventQueue.h"
#include "HardwareTimer.h"

Timer Timer_create(uint64_t interval_ms, TimerCallback callback)
{
  Timer timer;
  timer.interval_ms = interval_ms;
  timer.callback = callback;
  timer.last_time_ms = millis();
  timer.start = Timer_start;
  timer.stop = Timer_stop;
  return timer;
}

void Timer_start(Timer* timer)
{
  EventQueue_register_updater(EventQueue_instance(), Updater_create(timer, Timer_update));
}

void Timer_stop(Timer* timer)
{
  EventQueue_unregister_updater(EventQueue_instance(), Updater_create(timer, Timer_update));
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