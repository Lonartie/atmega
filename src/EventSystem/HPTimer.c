#include "HPTimer.h"
#include "EventQueue.h"
#include "HardwareTimer.h"

HPTimer HPTimer_create(uint64_t interval_us, HPTimerCallback callback)
{
  HPTimer timer;
  timer.interval_us = interval_us;
  timer.callback = callback;
  timer.last_time_us = micros();
  return timer;
}

void HPTimer_start(HPTimer* timer)
{
  EventQueue_register_updater(Updater_create(timer, HPTimer_update));
}

void HPTimer_update(void* obj)
{
  HPTimer* timer = (HPTimer*) obj;
  uint64_t current_time_us = millis();
  if (timer->last_time_us + timer->interval_us <= current_time_us) {
    timer->last_time_us = current_time_us;
    timer->callback();
  }
}