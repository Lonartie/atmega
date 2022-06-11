#include "Timer.h"
#include "EventQueue.h"
#include "HardwareTimer.h"

DEFINE_ACTOR(Timer);

DEFINE_ACTOR_FORWARDER(void, Timer, start);
DEFINE_ACTOR_FORWARDER(void, Timer, stop);

Timer Timer_create(uint64_t interval_ms, String event_type)
{
  Timer timer;
  timer.interval_ms = interval_ms;
  timer.last_time_ms = millis();
  timer.event_type = String_copy(event_type);
  
  SET_ACTOR_FOWARDER(timer, Timer, start);
  SET_ACTOR_FOWARDER(timer, Timer, stop);
  SET_ACTOR_MEM(timer, Timer);

  return timer;
}

void Timer_destroy(Timer* timer)
{
  // yes this should never be done
  // but we know this string wasn't const in the first place
  // so don't look at it :D
  free((char*)timer->event_type);
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
    Event event = Event_create(timer->event_type, 0, NULL);
    EventQueue_send_event(EventQueue_instance(), event);
  }
}