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

void Timer_update(void* timer) {
  Timer* actual_timer = (Timer*)timer;
  uint32_t current_time_ms = millis();
  if (actual_timer->last_time_ms + actual_timer->interval_ms <=
      current_time_ms) {
    actual_timer->last_time_ms = current_time_ms;
    Event event = Event_create(actual_timer->event);
    EventSystem_send_event(EventSystem_instance(), event);
  }
}