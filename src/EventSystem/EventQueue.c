#include "EventQueue.h"
#include "../Misc/utils.h"
#include <stdbool.h>
// #include <util/delay.h>

#define USE_EVENT_QUEUE_DELAY false
static const uint64_t EVENT_PERIOD_US MAYBE_UNUSED = 100;

DEFINE_ACTOR_FORWARDER_N(void, EventQueue, reg_updater, (Updater updater), (updater));
DEFINE_ACTOR_FORWARDER_N(void, EventQueue, unreg_updater, (Updater updater), (updater));
DEFINE_ACTOR_FORWARDER_N(void, EventQueue, reg_listener, (Listener listener), (listener));
DEFINE_ACTOR_FORWARDER_N(void, EventQueue, unreg_listener, (Listener listener), (listener));
DEFINE_ACTOR_FORWARDER_N(void, EventQueue, send_event, (Event event), (event));
DEFINE_ACTOR_FORWARDER(void, EventQueue, run);

EventQueue* EventQueue_instance()
{
  static EventQueue instance;
  static bool initialized = false;

  if (!initialized) 
  {
    instance.updaters = Vector_Updater_16_create();
    instance.events = Vector_Event_32_create();
    instance.listeners = Vector_Listener_64_create();

    SET_ACTOR_FORWARDER(instance, EventQueue, reg_updater);
    SET_ACTOR_FORWARDER(instance, EventQueue, unreg_updater);
    SET_ACTOR_FORWARDER(instance, EventQueue, reg_listener);
    SET_ACTOR_FORWARDER(instance, EventQueue, unreg_listener);
    SET_ACTOR_FORWARDER(instance, EventQueue, run);

    initialized = true;
  }

  return &instance;
}

void EventQueue_reg_updater(EventQueue* _this, Updater updater)
{
  Vector_Updater_16_push_back(&_this->updaters, updater);
}

void EventQueue_unreg_updater(EventQueue* _this, Updater updater)
{
  for (uint64_t i = 0; i < _this->updaters.size; i++)
    if (_this->updaters.data[i].update == updater.update && 
        _this->updaters.data[i].object == updater.object)
    {
      Vector_Updater_16_erase(&_this->updaters, i);
      return;
    }
}

void EventQueue_reg_listener(EventQueue* _this, Listener listener)
{
  Vector_Listener_64_push_back(&_this->listeners, listener);
}

void EventQueue_unreg_listener(EventQueue* _this, Listener listener)
{
  for (uint64_t i = 0; i < _this->listeners.size; i++)
    if (_this->listeners.data[i].callback == listener.callback && 
        String_equals(_this->listeners.data[i].event_type, listener.event_type))
    {
      Vector_Listener_64_erase(&_this->listeners, i);
      return;
    }
}

void EventQueue_send_event(EventQueue* _this, Event event)
{
  Vector_Event_32_push_back(&_this->events, event);
}

void EventQueue_run(EventQueue* _this)
{
  while (true)
  {
#if USE_EVENT_QUEUE_DELAY == true
    _delay_us(EVENT_PERIOD_US);
#endif

    // run updaters
    for (uint32_t i = 0; i < _this->updaters.size; i++) 
    {
      _this->updaters.data[i].update(_this->updaters.data[i].object);
    }

    Vector_Event_32 tmp_events = Vector_Event_32_move(&_this->events);

    // take all events
    for (uint64_t i = 0; i < tmp_events.size; ++i) 
    {
      Event event = tmp_events.data[i];

      // find listeners for this event
      for (uint64_t j = 0; j < _this->listeners.size; ++j) 
      {
        if (String_equals(event.event_type, _this->listeners.data[j].event_type)) 
        {
          _this->listeners.data[j].callback(event.event_data);
        }
      }
    }
  }
}