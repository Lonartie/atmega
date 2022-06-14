#include "Timer.h"
#include "EventSystem.h"
#include "HardwareTimer.h"

DEFINE_ACTOR_FORWARDER(void, Timer, start);
DEFINE_ACTOR_FORWARDER(void, Timer, stop);

Timer Timer_create(uint64_t interval_ms, String event_type)
{
  Timer timer;
  timer.interval_ms = interval_ms;
  timer.last_time_ms = millis();
  timer.event = event_type;
  
  SET_ACTOR_FORWARDER(timer, Timer, start);
  SET_ACTOR_FORWARDER(timer, Timer, stop);

  return timer;
}

void Timer_start(Timer* timer)
{
  EventSystem_reg_updater(EventSystem_instance(), Updater_create(timer, Timer_update));
}

void Timer_stop(Timer* timer)
{
  EventSystem_unreg_updater(EventSystem_instance(), Updater_create(timer, Timer_update));
}

void Timer_update(void* obj)
{
  Timer* timer = (Timer*) obj;
  uint64_t current_time_ms = millis();
  if (timer->last_time_ms + timer->interval_ms <= current_time_ms) {
    timer->last_time_ms = current_time_ms;
    Event event = Event_create(timer->event);
    EventSystem_send_event(EventSystem_instance(), event);
  }
}