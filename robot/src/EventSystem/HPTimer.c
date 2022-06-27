#include "HPTimer.h"
#include "EventSystem.h"
#include "HardwareTimer.h"

HPTimer HPTimer_create(uint64_t interval_us, String event)
{
  HPTimer timer;
  timer.interval_us = interval_us;
  timer.last_time_us = micros();
  timer.event = event;
  
  return timer;
}

void HPTimer_start(HPTimer* timer)
{
  EventSystem_reg_updater(EventSystem_instance(), Updater_create(timer, HPTimer_update));
}

void HPTimer_stop(HPTimer* timer)
{
  EventSystem_unreg_updater(EventSystem_instance(), Updater_create(timer, HPTimer_update));
}

void HPTimer_update(void* obj)
{
  HPTimer* timer = (HPTimer*) obj;
  uint64_t current_time_us = micros();
  if (timer->last_time_us + timer->interval_us <= current_time_us) {
    timer->last_time_us = current_time_us;
    Event event = Event_create(timer->event);
    EventSystem_send_event(EventSystem_instance(), event);
  }
}