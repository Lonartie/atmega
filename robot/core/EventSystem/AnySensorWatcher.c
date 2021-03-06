#include "AnySensorWatcher.h"

#include <stdarg.h>

#include "EventSystem.h"

AnySensorWatcher AnySensorWatcher_create(String event, uint8_t n, ...) {
  AnySensorWatcher watcher;
  watcher.event = event;
  va_list args;
  va_start(args, n);

  for (uint8_t i = 0; i < n; i++) {
    Pin pin = va_arg(args, Pin);
    Vector_Pin_8_push_back(&watcher.pins, pin);
    Vector__Bool_8_push_back(&watcher.states, false);
  }

  return watcher;
}

void AnySensorWatcher_start(AnySensorWatcher* _this) {
  EventSystem_reg_updater(EventSystem_instance(),
                          Updater_create(_this, &AnySensorWatcher_update));
}

void AnySensorWatcher_stop(AnySensorWatcher* _this) {
  EventSystem_unreg_updater(EventSystem_instance(),
                            Updater_create(_this, &AnySensorWatcher_update));
}

void AnySensorWatcher_update(void* _this) {
  AnySensorWatcher* this = (AnySensorWatcher*)_this;
  Vector_Pin_8* pins = &this->pins;
  Vector__Bool_8* states = &this->states;
  String event = this->event;

  bool any = false;
  for (uint8_t i = 0; i < pins->size; i++) {
    Pin* pin = Vector_Pin_8_get(pins, i);
    bool state = Pin_read(pin);

    if (state != *Vector__Bool_8_get(states, i)) {
      Vector__Bool_8_set(states, i, state);
      any = true;
    }
  }

  if (any) {
    EventSystem_send_event(EventSystem_instance(), Event_create(event));
  }
}