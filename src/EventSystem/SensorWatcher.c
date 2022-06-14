#include "SensorWatcher.h"
#include "EventSystem.h"
#include "../Misc/Debug.h"

DEFINE_ACTOR_FORWARDER(void, SensorWatcher, start);
DEFINE_ACTOR_FORWARDER(void, SensorWatcher, stop);

SensorWatcher SensorWatcher_create(Pin pin, String event)
{
  SensorWatcher watcher;
  watcher.pin = pin;
  watcher.event = event;
  watcher.last_state = false;

  SET_ACTOR_FORWARDER(watcher, SensorWatcher, start);
  SET_ACTOR_FORWARDER(watcher, SensorWatcher, stop);

  return watcher;
}

void SensorWatcher_start(SensorWatcher* _this)
{
  EventSystem_reg_updater(EventSystem_instance(), Updater_create(_this, &SensorWatcher_update));
}

void SensorWatcher_stop(SensorWatcher* _this)
{
  EventSystem_unreg_updater(EventSystem_instance(), Updater_create(_this, &SensorWatcher_update));
}

void SensorWatcher_update(void* _this)
{
  SensorWatcher* this = (SensorWatcher*)_this;
  Pin* pin = &this->pin;
  String event = this->event;
  bool state = Pin_read(pin);

  if (state != this->last_state)
  {
    this->last_state = state;
    EventSystem_send_event(EventSystem_instance(), Event_create(event));
  }
}