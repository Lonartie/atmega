#include "EventSystem.h"

#include <stdbool.h>

#include "Misc/Utils.h"

#define USE_EVENT_QUEUE_DELAY false
#if USE_EVENT_QUEUE_DELAY == true
#include <util/delay.h>
#endif

static const uint8_t EVENT_PERIOD_US MAYBE_UNUSED = 100;

EventSystem* EventSystem_instance() {
  static EventSystem instance;
  static bool initialized = false;

  if (!initialized) {
    instance.updaters = Vector_Updater_8_create();
    instance.listeners = Vector_Listener_8_create();
    instance.exit_flag = false;

    initialized = true;
  }

  return &instance;
}

void EventSystem_reg_updater(EventSystem* _this, Updater updater) {
  Vector_Updater_8_push_back(&_this->updaters, updater);
}

void EventSystem_unreg_updater(EventSystem* _this, Updater updater) {
  for (uint8_t i = 0; i < _this->updaters.size; i++)
    if (_this->updaters.data[i].update == updater.update &&
        _this->updaters.data[i].object == updater.object) {
      Vector_Updater_8_erase(&_this->updaters, i);
      return;
    }
}

void EventSystem_reg_listener(EventSystem* _this, Listener listener) {
  Vector_Listener_8_push_back(&_this->listeners, listener);
}

void EventSystem_unreg_listener(EventSystem* _this, Listener listener) {
  for (uint8_t i = 0; i < _this->listeners.size; i++)
    if (_this->listeners.data[i].callback == listener.callback &&
        String_equals(_this->listeners.data[i].event, listener.event)) {
      Vector_Listener_8_erase(&_this->listeners, i);
      return;
    }
}

void EventSystem_send_event(EventSystem* _this, Event event) {
  // find listeners for this event
  for (uint8_t j = 0; j < _this->listeners.size; ++j) {
    if (String_equals(event.event, _this->listeners.data[j].event)) {
      Listener* listener = &_this->listeners.data[j];
      if (listener->callback_d != NULL)
        listener->callback_d(listener->data);
      else if (listener->callback != NULL)
        listener->callback();
    }
  }
}

void EventSystem_run(EventSystem* _this) {
  while (!_this->exit_flag) {
#if USE_EVENT_QUEUE_DELAY == true
    _delay_us(EVENT_PERIOD_US);
#endif
    // run updaters
    for (uint8_t i = 0; i < _this->updaters.size; i++)
      _this->updaters.data[i].update(_this->updaters.data[i].object);
  }
}

void EventSystem_exit(EventSystem* _this) { _this->exit_flag = true; }