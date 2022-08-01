#include "Timer.h"

#include "EventSystem.h"
#include "HardwareTimer.h"

Timer Timer_create(uint32_t interval_ms, String event) {
  Timer timer;
  timer.interval_ms = interval_ms;
  timer.last_time_ms = millis();
  timer.event = event;

  return timer;
}

void Timer_start(Timer* timer) {
  EventSystem_reg_updater(EventSystem_instance(),
                          Updater_create(timer, Timer_update));
}

void Timer_stop(Timer* timer) {
  EventSystem_unreg_updater(EventSystem_instance(),
                            Updater_create(timer, Timer_update));
}

void Timer_update(void* obj) {
  Timer* timer = (Timer*)obj;
  uint32_t current_time_ms = millis();
  if (timer->last_time_ms + timer->interval_ms <= current_time_ms) {
    timer->last_time_ms = current_time_ms;
    Event event = Event_create(timer->event);
    EventSystem_send_event(EventSystem_instance(), event);
  }
}