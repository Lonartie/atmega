#include "PhasedTimer.h"

#include <stdarg.h>

#include "EventSystem.h"
#include "HardwareTimer.h"

PhasedTimer PhasedTimer_create(String event, uint8_t n, ...) {
  PhasedTimer timer;
  timer.event = event;
  timer.last_time_ms = millis();
  timer.phase = 0;
  timer.phases = Vector_long_8_create();

  va_list args;
  va_start(args, n);
  for (uint8_t i = 0; i < n; i++) {
    int arg = va_arg(args, int);
    Vector_long_8_push_back(&timer.phases, (long)arg);
  }
  va_end(args);

  return timer;
}

void PhasedTimer_start(PhasedTimer* timer) {
  EventSystem_reg_updater(EventSystem_instance(),
                          Updater_create(timer, PhasedTimer_update));
}

void PhasedTimer_stop(PhasedTimer* timer) {
  EventSystem_unreg_updater(EventSystem_instance(),
                            Updater_create(timer, PhasedTimer_update));
}

void PhasedTimer_update(void* obj) {
  PhasedTimer* timer = (PhasedTimer*)obj;
  uint64_t current_time_ms = millis();
  if (timer->last_time_ms + timer->phases.data[timer->phase] <=
      current_time_ms) {
    timer->last_time_ms = current_time_ms;
    timer->phase = (timer->phase + 1) % timer->phases.size;
    Event event = Event_create(timer->event);
    EventSystem_send_event(EventSystem_instance(), event);
  }
}