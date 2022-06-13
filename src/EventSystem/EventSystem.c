#include "EventSystem.h"
#include "../Misc/utils.h"
#include <stdbool.h>

#define USE_EVENT_QUEUE_DELAY false
#if USE_EVENT_QUEUE_DELAY == true
# include <util/delay.h>
#endif

static const uint64_t EVENT_PERIOD_US MAYBE_UNUSED = 100;

DEFINE_ACTOR_FORWARDER_N(void, EventSystem, reg_updater, (Updater updater), (updater));
DEFINE_ACTOR_FORWARDER_N(void, EventSystem, unreg_updater, (Updater updater), (updater));
DEFINE_ACTOR_FORWARDER_N(void, EventSystem, reg_listener, (Listener listener), (listener));
DEFINE_ACTOR_FORWARDER_N(void, EventSystem, unreg_listener, (Listener listener), (listener));
DEFINE_ACTOR_FORWARDER_N(void, EventSystem, send_event, (Event event), (event));
DEFINE_ACTOR_FORWARDER(void, EventSystem, run);
DEFINE_ACTOR_FORWARDER(void, EventSystem, exit);

EventSystem* EventSystem_instance()
{
  static EventSystem instance;
  static bool initialized = false;

  if (!initialized) 
  {
    instance.updaters = Vector_Updater_16_create();
    instance.listeners = Vector_Listener_64_create();
    instance.exit_flag = false;

    SET_ACTOR_FORWARDER(instance, EventSystem, reg_updater);
    SET_ACTOR_FORWARDER(instance, EventSystem, unreg_updater);
    SET_ACTOR_FORWARDER(instance, EventSystem, reg_listener);
    SET_ACTOR_FORWARDER(instance, EventSystem, unreg_listener);
    SET_ACTOR_FORWARDER(instance, EventSystem, send_event);
    SET_ACTOR_FORWARDER(instance, EventSystem, run);
    SET_ACTOR_FORWARDER(instance, EventSystem, exit);

    initialized = true;
  }

  return &instance;
}

void EventSystem_reg_updater(EventSystem* _this, Updater updater)
{
  ACTOR(_this->updaters).push_back(updater);
}

void EventSystem_unreg_updater(EventSystem* _this, Updater updater)
{
  for (uint64_t i = 0; i < _this->updaters.size; i++)
    if (_this->updaters.data[i].update == updater.update && 
        _this->updaters.data[i].object == updater.object)
    {
      ACTOR(_this->updaters).erase(i);
      return;
    }
}

void EventSystem_reg_listener(EventSystem* _this, Listener listener)
{
  ACTOR(_this->listeners).push_back(listener);
}

void EventSystem_unreg_listener(EventSystem* _this, Listener listener)
{
  for (uint64_t i = 0; i < _this->listeners.size; i++)
    if (_this->listeners.data[i].callback == listener.callback && 
        String_equals(_this->listeners.data[i].event_type, listener.event_type))
    {
      ACTOR(_this->listeners).erase(i);
      return;
    }
}

void EventSystem_send_event(EventSystem* _this, Event event)
{
  // find listeners for this event
  for (uint64_t j = 0; j < _this->listeners.size; ++j) 
  {
    if (String_equals(event.event_type, _this->listeners.data[j].event_type)) 
    {
      Listener* listener = &_this->listeners.data[j];
      if (listener->callback_r != NULL)
        listener->callback_r(listener->receiver);
      else if (listener->callback != NULL)
        listener->callback();
    }
  }
}

void EventSystem_run(EventSystem* _this)
{
  while (!_this->exit_flag)
  {
#if USE_EVENT_QUEUE_DELAY == true
    _delay_us(EVENT_PERIOD_US);
#endif
    // run updaters
    for (uint32_t i = 0; i < _this->updaters.size; i++)
      _this->updaters.data[i].update(_this->updaters.data[i].object);
  }
}

void EventSystem_exit(EventSystem* _this)
{
  _this->exit_flag = true;
}